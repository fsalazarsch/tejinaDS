#pragma once

#include <stdio.h>
#include <nds.h>
#include <dsf.h>

#include "font_0_256.h"
#include "font2_0_16bit.h"
#include "nunito_0.h"
#include "kana_0.h"
#include "keyboard.h"

#include "font_fnt_bin.h"
#include "font2_fnt_bin.h"
#include "nunito_fnt_bin.h"
#include "kana_fnt_bin.h"
#include "keyboard_fnt_bin.h"

#include "backgrounds.h"

static dsf_handle handle;
static dsf_handle handle2;
static dsf_handle handle_nunito;
static dsf_handle handle_keyboard;
static dsf_handle handle_kana;

__attribute__((noreturn)) void wait_forever(void)
{
    while (1)
    {
        swiWaitForVBlank();
        scanKeys();
        if (keysHeld() & KEY_START)
            exit(1);
    }
}

static void libdsf_fonts_load(void)
{
    dsf_error rc;
    rc = DSF_LoadFontMemory(&handle, font_fnt_bin, font_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() 1 failed: %d\n", rc); wait_forever(); }
    rc = DSF_LoadFontMemory(&handle2, font2_fnt_bin, font2_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() 2 failed: %d\n", rc); wait_forever(); }
    rc = DSF_LoadFontMemory(&handle_nunito, nunito_fnt_bin, nunito_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() nunito failed: %d\n", rc); wait_forever(); }

    rc = DSF_LoadFontMemory(&handle_kana, kana_fnt_bin, kana_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() kana failed: %d\n", rc); wait_forever(); }

    rc = DSF_LoadFontMemory(&handle_keyboard, keyboard_fnt_bin, keyboard_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() kbd failed: %d\n", rc); wait_forever(); }

}

static void libdsf_fonts_free(void)
{
    dsf_error rc;
    rc = DSF_FreeFont(&handle);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() 1 failed: %d\n", rc); wait_forever(); }
    rc = DSF_FreeFont(&handle2);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() 2 failed: %d\n", rc); wait_forever(); }
    rc = DSF_FreeFont(&handle_nunito);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() nunito failed: %d\n", rc); wait_forever(); }
    rc = DSF_FreeFont(&handle_kana);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() kana failed: %d\n", rc); wait_forever(); }
    rc = DSF_FreeFont(&handle_keyboard);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() kbd failed: %d\n", rc); wait_forever(); }

}

void draw_text_font(int bg, int x, int y, const char *text,
                    dsf_handle font_handle,
                    const void *bitmap, u16 color_texto)
{
    void *out_texture;
    size_t out_width, out_height;

    dsf_error r = DSF_StringRenderToTexture(
        font_handle,
        text,
        GL_RGBA,
        bitmap, 512, 512,
        &out_texture, &out_width, &out_height
    );

    if (r != DSF_NO_ERROR)
        return;

    uint16_t *bg_ptr = bgGetGfxPtr(bg);
    uint16_t *src = (uint16_t*)out_texture;

    const int bg_width = 512;

    for (int j = 0; j < out_height; j++)
    {
        uint16_t *dst = bg_ptr + ((j + y) * bg_width + x);
        uint16_t *line = src + j * out_width;

        for (int i = 0; i < out_width; i++)
        {
            uint16_t pixel = line[i];

            if (pixel & 0x8000) // alpha
                dst[i] = color_texto;
        }
    }

    free(out_texture);
}