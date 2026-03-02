#pragma once

#include <stdio.h>
#include <nds.h>
#include <dsf.h>

#include "font_0_256.h"
#include "font2_0_16bit.h"
#include "tloz_0.h"
#include "keyboard.h"

#include "font_fnt_bin.h"
#include "font2_fnt_bin.h"
#include "tloz_fnt_bin.h"
#include "keyboard_fnt_bin.h"

#include "backgrounds.h"

static dsf_handle handle;
static dsf_handle handle2;
static dsf_handle handle_tloz;
static dsf_handle handle_keyboard;

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
    rc = DSF_LoadFontMemory(&handle_tloz, tloz_fnt_bin, tloz_fnt_bin_size);
    if (rc != DSF_NO_ERROR) { printf("DSF_LoadFontMemory() tloz failed: %d\n", rc); wait_forever(); }

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
    rc = DSF_FreeFont(&handle_tloz);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() tloz failed: %d\n", rc); wait_forever(); }
    rc = DSF_FreeFont(&handle_keyboard);
    if (rc != DSF_NO_ERROR) { printf("DSF_FreeFont() kbd failed: %d\n", rc); wait_forever(); }

}
