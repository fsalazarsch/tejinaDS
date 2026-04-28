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



int unicode_to_utf8(unsigned int codepoint, char* out);
void draw_kanji_detail(C3D_RenderTarget *bottom, C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, TablaState *estado);



void mostrar_tabla_kanji(C3D_RenderTarget *top, C3D_RenderTarget *bottom, 
                   C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, 
                   TablaState *estado); //user la categoria como paginador

void handle_tabla_touch_kanji(u32 kDown, u32 kHeld, u32 kUp, int tx, int ty, TablaState *estado);


#endif