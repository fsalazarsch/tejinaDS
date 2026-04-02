#ifndef TTS_ENGINE_H
#define TTS_ENGINE_H

#include <3ds.h>
#include <string.h>
#include <stdbool.h>
#include "tts_3ds.h"

// ================= CONFIG =================
#define TTS_STACK_SIZE  (256 * 1024)  // 256KB
#define TTS_PRIO        0x18

#define TTS_MAX_QUEUE   4
#define TTS_MAX_TEXT    128
#define TTS_MAX_ACCENT  32

// ================= STRUCT =================

typedef struct {
    TTS_3DS tts;
    Thread thread;
    LightLock lock;
    bool running;
    bool initialized;
    char queue_text[TTS_MAX_QUEUE][TTS_MAX_TEXT];
    char queue_accent[TTS_MAX_QUEUE][TTS_MAX_ACCENT];
    int head;
    int tail;
} TTSEngine;

static TTSEngine g_tts;

// ================= INTERNAL =================
static void tts_engine_thread(void* arg) {


    while (g_tts.running) {

        LightLock_Lock(&g_tts.lock);

        if (g_tts.head != g_tts.tail) {
            char text[TTS_MAX_TEXT]; 
            strcpy(text, g_tts.queue_text[g_tts.head]);
            g_tts.head = (g_tts.head + 1) % TTS_MAX_QUEUE;

            LightLock_Unlock(&g_tts.lock);

            const char* acc = g_tts.queue_accent[g_tts.head][0] ? 
                  g_tts.queue_accent[g_tts.head] : NULL;

            tts_speak(&g_tts.tts, text, acc);

        } else {
            LightLock_Unlock(&g_tts.lock);
            svcSleepThread(10000000); // 10ms
        }
    }
}

// ================= API =================

static bool tts_engine_init(const char* voice_path) {

    memset(&g_tts, 0, sizeof(g_tts));
    LightLock_Init(&g_tts.lock);

    int ok = tts_init(&g_tts.tts, voice_path);

    if (!ok) {
        printf("TTS INIT FAIL\n");
        return false;
    }

    g_tts.running = true;
    g_tts.thread = threadCreate(
        tts_engine_thread,
        NULL,
        TTS_STACK_SIZE,
        TTS_PRIO,
        -2,
        false
    );

    if (!g_tts.thread) {
        printf("THREAD FAIL\n");
        return false;
    }

    g_tts.initialized = true;
    return true;
}

static void tts_engine_speak(const char* text, const char* accent) {
    if (!g_tts.initialized) return;
    LightLock_Lock(&g_tts.lock);
    int next = (g_tts.tail + 1) % TTS_MAX_QUEUE;
    if (next != g_tts.head) {
        strncpy(g_tts.queue_text[g_tts.tail], text, TTS_MAX_TEXT - 1);
        if (accent)
            strncpy(g_tts.queue_accent[g_tts.tail], accent, TTS_MAX_ACCENT - 1);
        else
            g_tts.queue_accent[g_tts.tail][0] = '\0';
        g_tts.tail = next;
    }
    LightLock_Unlock(&g_tts.lock);
}

static void tts_engine_exit() {

    if (!g_tts.initialized) return;

    g_tts.running = false;

    if (g_tts.thread) {
        threadJoin(g_tts.thread, U64_MAX);
        threadFree(g_tts.thread);
    }

    tts_free(&g_tts.tts);
    g_tts.initialized = false;
}

#endif