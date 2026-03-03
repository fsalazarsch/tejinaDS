// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024-2026

#include <stdio.h>
#include <nds.h>
#include "loadFonts.h"




void draw_screen_main(void)
{
    int bg_main = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 4, 0);

    // Load the palette used by the font
    memcpy(BG_PALETTE, font_0_256Pal, font_0_256PalLen);

    // Set the last color of the palette to white (for the console)
    BG_PALETTE[255] = RGB5(31, 31, 31);

    setBackdropColor(RGB5(10, 5, 10));

    void *out_texture;
    size_t out_width, out_height;

    dsf_error r = DSF_StringRenderToTexture(handle,
                            "VAWATa\ntajl", GL_RGB256 ,
                            font_0_256Bitmap, 256, 256,
                            &out_texture, &out_width, &out_height);
    if (r != DSF_NO_ERROR)
    {
        printf("DSF_StringRenderToTexture() failed: %d", r);
        wait_forever();
    }

    // X must be a multiple of two
    const int x = 20, y = 50;

    for (int j = 0; j < out_height; j++)
    {
        const uint16_t *src = out_texture;
        uint16_t *dst = bgGetGfxPtr(bg_main);

        const int bg_width = 256;
        dst += ((j + y) * bg_width + x) / 2;
        src += j * out_width / 2;

        for (int i = 0; i < out_width; i += 2)
            *dst++ = *src++;
    }

    free(out_texture);
}

void draw_screen_sub(void)
{
    int bg_sub = bgInitSub(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    BG_PALETTE_SUB[255] = RGB5(31, 31, 31);

    setBackdropColorSub(clrLightBlue);


    const char *menu_options[] = {
        "(Y) Vocabulario inicial",
        "(X) Vocabulario intermedio",
        "(A) Kanji básicos",
        "(B) Configuración"
    };

    draw_menu(bg_sub, handle_tloz, tloz_0Bitmap,
              menu_options,
              sizeof(menu_options) / sizeof(menu_options[0]));

    //draw_menu(bg_sub, handle_tloz, tloz_0Bitmap );

    //draw_button(fb, 10, 13, SCREEN_WIDTH_DS-30, SCREEN_HEIGHT_DS/7, 2,
    //            clrLightBlue, clrBlue,
    //            handle_tloz, tloz_0Bitmap, 256, 256,
    //            "(Y) Vocabulario inicial", 20, 20);


}

int main(int argc, char *argv[])
{
    // Mode 5 lets you use layer 2 as an 8 or 16-bit bitmap
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);

    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD,
                        VRAM_C_SUB_BG, VRAM_D_LCD);

    // Setup sub screen for the text console
    consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 7, 0, true, true);

    libdsf_fonts_load();
    draw_screen_main();
    draw_screen_sub();

    consoleSetCursor(NULL, 0, 0);
    printf("This example uses 8-bit and\n16-bit backgrounds.\n\n");
    consoleSetCursor(NULL, 0, 23);
    printf("START: Exit to loader");

    while (1)
    {
        swiWaitForVBlank();

        scanKeys();
        if (keysHeld() & KEY_START)
            break;
    }

    libdsf_fonts_free();

    return 0;
}
