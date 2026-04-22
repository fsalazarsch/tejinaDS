#ifndef KANJI_TABLE_SCREEN_H
#define KANJI_TABLE_SCREEN_H

#include "tabla.h"
#include "themes.h"
#include "kanji_data.h"
#include "drawing.h"





extern ThemeID currentTheme;




#define X_INICIAL  20
#define Y_INICIAL  50
#define ANCHO_CELDA 36
#define ALTO_CELDA  34



int unicode_to_utf8(unsigned int codepoint, char* out) {
    if (codepoint <= 0x7F) {
        out[0] = (char)codepoint;
        out[1] = '\0';
        return 1;
    } else if (codepoint <= 0x7FF) {
        out[0] = (char)(0xC0 | (codepoint >> 6));
        out[1] = (char)(0x80 | (codepoint & 0x3F));
        out[2] = '\0';
        return 2;
    } else if (codepoint <= 0xFFFF) {
        // Aquí caen todos los kanji comunes (CJK básico)
        out[0] = (char)(0xE0 | (codepoint >> 12));
        out[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[2] = (char)(0x80 | (codepoint & 0x3F));
        out[3] = '\0';
        return 3;
    } else {
        // Plano suplementario (kanji raros, no deberías llegar aquí)
        out[0] = (char)(0xF0 | (codepoint >> 18));
        out[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[3] = (char)(0x80 | (codepoint & 0x3F));
        out[4] = '\0';
        return 4;
    }
}









void draw_kanji_detail(C3D_RenderTarget *bottom, C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, TablaState *estado)
{
   if(estado->categoria != 2 && estado->categoria != 5){
    DrawRoundedRect(80, 32, 160, 150, 4, themes[currentTheme].cellIdle);
    DrawRoundedRect(80, 32, 160, 150, 4, 0x00000000);
    DrawRoundedRect(159, 32, 2, 150, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(80, 106, 160, 2, 0, themes[currentTheme].borderCell);

    if (!kanaOculto) {
        if (mostrandoTrazos) {
            //int done = animate_kanji_at(&k, &anim, 90.0f, 35.0f, 130.0f);
            
        } else {
            // carga estática solo cuando NO está animando
            //KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];
            //kanji_load(entry.codepoints[0], &k);
            //draw_kanji_static_at(&k, 90.0f, 35.0f, 130.0f);
        }
    }
    
    drawing_draw(themes[currentTheme].kanaText);

   } else {
    DrawRoundedRect(20, 32, 270, 150, 4, themes[currentTheme].cellIdle);
    DrawRoundedRect(20, 32, 270, 150, 4, 0x00000000);
    DrawRoundedRect(99, 32, 2, 150, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(20, 106, 270, 2, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(220, 32, 2, 150, 0, themes[currentTheme].borderCell);

    if (!kanaOculto) {
        if (mostrandoTrazos) {
            //int done1 = animate_kanji_at(&k,  &anim,  30.0f, 35.0f, 130.0f);
        } else {
            //KanaEntry entry = get_tabla_kanji(1);
            //kanji_load(entry.codepoints[0], &k);
            //draw_kanji_static_at(&k,  30.0f, 35.0f, 130.0f);
        }
    }
    drawing_draw(themes[currentTheme].kanaText);
   }

    // --- TÍTULO ROMAJI ---
    C2D_Text romajiText;
    C2D_TextFontParse(&romajiText, font2, g_staticBuf, get_tabla(estado->categoria, estado->fila)[estado->col].romaji);
    C2D_TextOptimize(&romajiText);
    C2D_DrawText(&romajiText, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 24.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(270, 10, 30, 30, 6, themes[currentTheme].cellIdle);
    C2D_Text btnHelp;
    C2D_TextFontParse(&btnHelp, font2, g_staticBuf, "?");
    C2D_TextOptimize(&btnHelp);
    C2D_DrawText(&btnHelp, C2D_AtBaseline | C2D_AlignCenter, 285.0f, 30.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(10, 197, 95, 36, 8, themes[currentTheme].btnAudio);
    C2D_Text btnAudio;
    C2D_TextFontParse(&btnAudio, font2, g_staticBuf, "Audio");
    C2D_TextOptimize(&btnAudio);
    C2D_DrawText(&btnAudio, C2D_AtBaseline | C2D_AlignCenter, 57.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].btnAudioText);

    DrawRoundedRect(110, 197, 95, 36, 8, themes[currentTheme].cellIdle);
    C2D_Text btnOcultar;
    C2D_TextFontParse(&btnOcultar, font2, g_staticBuf, kanaOculto ? "Mostrar" : "Ocultar");
    C2D_TextOptimize(&btnOcultar);
    C2D_DrawText(&btnOcultar, C2D_AtBaseline | C2D_AlignCenter, 157.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(210, 197, 95, 36, 8, themes[currentTheme].btnAudio);
    C2D_Text btnTrazos;
    C2D_TextFontParse(&btnTrazos, font2, g_staticBuf, "Trazos");
    C2D_TextOptimize(&btnTrazos);
    C2D_DrawText(&btnTrazos, C2D_AtBaseline | C2D_AlignCenter, 257.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    if (showHelp) {
        DrawRoundedRect(20, 40, 280, 140, 10, themes[currentTheme].cellIdle);
        C2D_Text helpText;
        C2D_TextFontParse(&helpText, font2, g_staticBuf,
            "Uso:\n\n"
            "- Kana grande: lectura principal\n"
            "- Boton Audio: escuchar\n"
            "- Trazos: orden de escritura\n"
            "- L/R: cambiar tabla\n"
            "- D-Pad: mover cursor\n"
        );
        C2D_TextOptimize(&helpText);
        C2D_DrawText(&helpText, C2D_AtBaseline, 30, 60, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
    }
}



void mostrar_tabla_kanji(C3D_RenderTarget *top, C3D_RenderTarget *bottom, 
                   C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, 
                   TablaState *estado)
{
    //PANTALLA SUPERIOR
    C2D_SceneBegin(top);

    int max_filas = 5;
    int max_columnas = 10;
    int ancho_celda = ANCHO_CELDA;
    int idx = 0;
    for (int i = 0; i < max_filas; i++) {        // filas
        for (int j = 0; j < max_columnas; j++) {   // columnas


            int cx = X_INICIAL + j * ancho_celda;
            int cy = Y_INICIAL + i * ALTO_CELDA;

            // cursor resaltado
            if (i == estado->fila && j == estado->col)
                DrawRoundedRect(cx - 6, cy - ALTO_CELDA + 8, ancho_celda, ALTO_CELDA, 5, themes[currentTheme].cellCursor);

            u32 color = (i == estado->fila && j == estado->col) 
            ? themes[currentTheme].bg 
            : themes[currentTheme].kanaText;
            

            char kanji_utf8[5];
            unicode_to_utf8(kanji_data[idx].unicode, kanji_utf8);

            C2D_Text kanaText;
            C2D_TextFontParse(&kanaText, font1, g_staticBuf, kanji_utf8);
            C2D_TextOptimize(&kanaText);
            C2D_DrawText(&kanaText, C2D_AtBaseline, cx, cy, 0.0f, 1.2f, 1.2f, color);
            idx ++;
        }
    }
    C2D_SceneBegin(bottom);
    if (tablaState.seleccionado) {
        // mostrar detalle del kana actual
        //draw_kana_detail(bottom, g_staticBuf, font1, font2, estado);
    
    } else {
    // PANTALLA INFERIOR
    

    char debug[100];
    snprintf(debug, sizeof(debug), "fila:%d col:%d cat:%d",
             estado->fila, estado->col, estado->categoria);

    C2D_Text debugText;
    C2D_TextFontParse(&debugText, font2, g_staticBuf, debug);
    C2D_TextOptimize(&debugText);
    C2D_DrawText(&debugText, C2D_AtBaseline, 20, 50, 0.0f, 0.8f, 0.8f, themes[currentTheme].kanaText);
    }
}

static inline void handle_tabla_touch_kanji(u32 kDown, u32 kHeld, u32 kUp, int tx, int ty, TablaState *estado)
{
    
    //KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];


    //drawing_update(kHeld, kUp, tx, ty, ax, 32, aw, 150);

    if (!(kDown & KEY_TOUCH)) return;
    
    
    // --- BOTÓN AUDIO ---
    if (isTouchInRect(tx, ty, 10, 197, 95, 36)) {
        
    }

    // --- BOTÓN OCULTAR ---
    if (isTouchInRect(tx, ty, 110, 197, 95, 36)) {
        kanaOculto = !kanaOculto;
    }

    // --- BOTÓN TRAZOS ---
if (isTouchInRect(tx, ty, 210, 197, 95, 36)) {
    kanaOculto = false;
    mostrandoTrazos = !mostrandoTrazos;
    if (mostrandoTrazos) {
        //kanji_load(entry.codepoints[0], &k);
        //kanji_anim_init(&anim);
    }
}

    // --- BOTÓN HELP ---
    if (isTouchInRect(tx, ty, 270, 10, 30, 30)) {
        showHelp = !showHelp;
    }

}



#endif