#ifndef KANA_TABLE_SCREEN_H
#define KANA_TABLE_SCREEN_H


#include "tabla.h"
#include "themes.h"
#include "kanji_stroke_data.h"
#include "tts_engine.h"
#include "drawing.h"



#define X_INICIAL  20
#define Y_INICIAL  50
#define ANCHO_CELDA 36
#define ALTO_CELDA  34

extern const KanaEntry hiragana[5][10];
extern const KanaEntry hiragana_dakuten[5][5];
extern const KanaEntry hiragana_comb[6][6];
extern const KanaEntry katakana[5][10];
extern const KanaEntry katakana_dakuten[5][5];
extern const KanaEntry katakana_comb[6][6];

void draw_kana_detail(C3D_RenderTarget *bottom,
                      C2D_TextBuf g_staticBuf,
                      C2D_Font font1,
                      C2D_Font font2,
                      TablaState *estado);

void handle_tabla_touch(u32 kDown, u32 kHeld, u32 kUp, int tx, int ty, TablaState *estado);
void mostrar_tabla(C3D_RenderTarget*, C3D_RenderTarget*, C2D_TextBuf, C2D_Font, C2D_Font, TablaState*);

#endif



