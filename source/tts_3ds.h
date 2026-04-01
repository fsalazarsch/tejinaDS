/*
 * tts_3ds.h — TTS japones para 3DS
 * Usa libhtsengine + voz nitech desde romfs
 *
 * USO:
 *   TTS_3DS tts;
 *   tts_init(&tts, "romfs:/nitech_jp_atr503_m001.htsvoice");
 *   tts_speak(&tts, "こんにちは");
 *   tts_free(&tts);
 */

#ifndef TTS_3DS_H
#define TTS_3DS_H

#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "HTS_engine.h"
#include "hiragana_phoneme.h"

#define TTS_SAMPLE_RATE   22050
#define TTS_FRAME_PERIOD  110
#define TTS_LABEL_FMT \
    "xx^xx-%s+xx=xx/A:0+1+1/B:xx-xx_xx/C:xx_xx+xx/D:xx+xx_xx/" \
    "E:xx_xx!xx_xx-xx/F:xx_xx#xx_xx@xx_xx|xx_xx/" \
    "G:xx_xx%%xx_xx_xx/H:xx_xx/I:xx-xx@xx+xx&xx-xx|xx+xx/J:1_1/K:1+1-1"

typedef struct {
    HTS_Engine engine;
    short     *pcm;
    size_t     pcm_len;
    int        ready;
} TTS_3DS;

static int _tts_make_labels(const char *hira, char ***out) {
    char ph[2048];
    int  n = hiragana_str_to_phonemes(hira, ph, sizeof(ph));
    if (n <= 0) return -1;
    char  *tmp = (char*)malloc(strlen(ph) + 1);
    char **tok = (char**)malloc(sizeof(char*) * (size_t)n);
    if (!tmp || !tok) { free(tmp); free(tok); return -1; }
    strcpy(tmp, ph);
    int c = 0;
    char *t = strtok(tmp, " ");
    while (t && c < n) { tok[c++] = t; t = strtok(NULL, " "); }
    char **labels = (char**)malloc(sizeof(char*) * (size_t)c);
    if (!labels) { free(tmp); free(tok); return -1; }
    for (int i = 0; i < c; i++) {
        labels[i] = (char*)malloc(512);
        if (!labels[i]) {
            for (int j = 0; j < i; j++) free(labels[j]);
            free(labels); free(tmp); free(tok); return -1;
        }
        snprintf(labels[i], 512, TTS_LABEL_FMT, tok[i]);
    }
    free(tmp); free(tok);
    *out = labels;
    return c;
}

static void _tts_free_labels(char **labels, int n) {
    if (!labels) return;
    for (int i = 0; i < n; i++) free(labels[i]);
    free(labels);
}

/*
 * tts_init — carga la voz desde romfs
 * voice_path: "romfs:/nitech_jp_atr503_m001.htsvoice"
 */
static inline int tts_init(TTS_3DS *tts, const char *voice_path) {
    if (!tts || !voice_path) return 0;
    memset(tts, 0, sizeof(TTS_3DS));
    HTS_Engine_initialize(&tts->engine);
    char *voices[1] = { (char*)voice_path };
    if (!HTS_Engine_load(&tts->engine, voices, 1)) {
        HTS_Engine_clear(&tts->engine);
        return 0;
    }
    HTS_Engine_set_sampling_frequency(&tts->engine, TTS_SAMPLE_RATE);
    HTS_Engine_set_fperiod           (&tts->engine, TTS_FRAME_PERIOD);
    HTS_Engine_set_audio_buff_size   (&tts->engine, 0);
    HTS_Engine_set_volume            (&tts->engine, 1.0);
    HTS_Engine_set_speed             (&tts->engine, 1.0);
    HTS_Engine_set_alpha             (&tts->engine, 0.55);
    HTS_Engine_set_beta              (&tts->engine, 0.0);
    HTS_Engine_set_msd_threshold     (&tts->engine, 1, 0.5);
    tts->ready = 1;
    return 1;
}

/*
 * tts_speak — sintetiza hiragana y reproduce via ndsp
 */
static inline int tts_speak(TTS_3DS *tts, const char *hiragana) {
    if (!tts || !tts->ready || !hiragana) return 0;
    if (tts->pcm) { free(tts->pcm); tts->pcm = NULL; tts->pcm_len = 0; }
    char **labels = NULL;
    int n = _tts_make_labels(hiragana, &labels);
    if (n <= 0) return 0;
    HTS_Boolean ok = HTS_Engine_synthesize_from_strings(
                         &tts->engine, labels, (size_t)n);
    _tts_free_labels(labels, n);
    if (!ok) { HTS_Engine_refresh(&tts->engine); return 0; }
    size_t ns = HTS_Engine_get_nsamples(&tts->engine);
    if (!ns) { HTS_Engine_refresh(&tts->engine); return 0; }
    /* Alinear a 16 bytes para ndsp */
    size_t buf_size = (ns * sizeof(short) + 15) & ~15;
    tts->pcm = (short*)linearAlloc(buf_size);
    if (!tts->pcm) { HTS_Engine_refresh(&tts->engine); return 0; }
    for (size_t i = 0; i < ns; i++) {
        double s = HTS_Engine_get_generated_speech(&tts->engine, i);
        if      (s >  32767.0) s =  32767.0;
        else if (s < -32768.0) s = -32768.0;
        tts->pcm[i] = (short)s;
    }
    tts->pcm_len = ns;
    HTS_Engine_refresh(&tts->engine);
    /* Reproducir con ndsp canal 0 */
    ndspChnReset(0);
    ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
    ndspChnSetRate(0, (float)TTS_SAMPLE_RATE);
    ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM16);
    ndspWaveBuf wb;
    memset(&wb, 0, sizeof(wb));
    wb.data_vaddr = tts->pcm;
    wb.nsamples   = (u32)ns;
    wb.looping    = false;
    wb.status     = NDSP_WBUF_FREE;
    DSP_FlushDataCache(tts->pcm, buf_size);
    ndspChnWaveBufAdd(0, &wb);
    /* Esperar que termine */
    while (wb.status != NDSP_WBUF_DONE) svcSleepThread(1000000);
    return 1;
}

/*
 * tts_set_speed — velocidad (1.0 = normal)
 */
static inline void tts_set_speed(TTS_3DS *tts, double speed) {
    if (tts && tts->ready) HTS_Engine_set_speed(&tts->engine, speed);
}

/*
 * tts_set_pitch — tono en semitonos (0.0 = normal)
 */
static inline void tts_set_pitch(TTS_3DS *tts, double semitones) {
    if (tts && tts->ready) HTS_Engine_add_half_tone(&tts->engine, semitones);
}

/*
 * tts_free — libera recursos
 */
static inline void tts_free(TTS_3DS *tts) {
    if (!tts) return;
    if (tts->pcm)   linearFree(tts->pcm);
    if (tts->ready) HTS_Engine_clear(&tts->engine);
    memset(tts, 0, sizeof(TTS_3DS));
}

#endif /* TTS_3DS_H */