/*
 * =========================================================
 *   help_screen.c  —  pantalla de ayuda estilo lección
 *
 *   Overlay en pantalla superior (400x240) que recibe un
 *   string por parámetro y lo despliega con efecto tipo
 *   máquina, junto a un portrait. No borra lo ya dibujado.
 * =========================================================
 */

#include <citro2d.h>
#include <string.h>

#include "help_screen.h"
#include "..\functions.h"
#include "..\themes.h"

extern C2D_Font fontdialog;
extern C2D_SpriteSheet portraits[5];
extern C3D_RenderTarget *top;

/* ---------------------------------------------------------
   COLORES (igual estilo que renderer.c)
   --------------------------------------------------------- */
#define COL_WHITE   C2D_Color32(255, 255, 255, 255)
#define COL_YELLOW  C2D_Color32(255, 220,  80, 255)
#define COL_CYAN    C2D_Color32( 80, 220, 255, 255)
#define COL_RED     C2D_Color32(128,   0, 128, 255)
#define COL_GREEN   C2D_Color32(  0, 100,   0, 255)
#define COL_BLACK   C2D_Color32(  0,   0,   0, 255)
#define COL_GRAY    C2D_Color32(180, 180, 180, 255)
#define COL_BG_DARK C2D_Color32( 20,  20,  20,  50)
#define COL_BG_DARK2 C2D_Color32(200, 188, 165, 255)

#define Z_DIM     0.90f
#define Z_BOX     0.91f
#define Z_BOX_IN  0.92f
#define Z_DETAIL  0.93f
#define Z_PORTRAIT 0.95f
#define Z_TEXT    0.97f

/* ---------------------------------------------------------
   ESTADO
   --------------------------------------------------------- */
static int          help_active  = 0;
static char         help_text[512];
static C2D_TextBuf  help_buf     = NULL;

/* ---------------------------------------------------------
   TEXT (usa su propio buffer, no toca g_dynamicBuf)
   --------------------------------------------------------- */
static void draw_text_h(const char* str, float x, float y, float sz, u32 color, float wrap)
{
    if (!str || str[0] == '\0') return;

    C2D_Text t;
    C2D_TextFontParse(&t, fontdialog, help_buf, str);
    C2D_TextOptimize(&t);
    C2D_DrawText(&t, C2D_WithColor | C2D_WordWrap, x, y, Z_TEXT, sz, sz, color, wrap);
}

/* =========================================================
   API
   ========================================================= */

void help_show(const char* text)
{
    if (!text) text = "";

    strncpy(help_text, text, 511);
    help_text[511] = '\0';

    if (!help_buf) help_buf = C2D_TextBufNew(4096);

    help_active = 1;
}

void help_hide(void)
{
    help_active = 0;
}

void help_toggle(const char* text)
{
    if (help_active) help_hide();
    else             help_show(text);
}

int help_is_active(void)
{
    return help_active;
}

void help_render(void)
{
    if (!help_active) return;
    if (!help_buf) help_buf = C2D_TextBufNew(4096);

    C2D_TextBufClear(help_buf);
    C2D_SceneBegin(top);

    C2D_DrawRectSolid(0, 0, Z_DIM, 400, 240, C2D_Color32(0, 0, 0, 150));

    /* cuadro principal: padding inferior = superior (18px) */
    DrawRoundedRectZ(20, 18, 360, 204, 10, COL_BLACK, Z_BOX);
    DrawRoundedRectZ(22, 20, 356, 200, 10, COL_BG_DARK2, Z_BOX_IN);

    draw_text_h("Ayuda", 30, 22, 0.6f, COL_YELLOW, 0.0f);

    if (portraits[0])
    {
        C2D_Image img = C2D_SpriteSheetGetImage(portraits[0], 0);
        C2D_DrawImageAt(img, 28, 45, Z_PORTRAIT, NULL, 0.55f, 0.55f);
    }

    DrawRoundedRectZ(130, 45, 230, 160, 8, COL_WHITE, Z_DETAIL);

    /* wrap = ancho útil del recuadro blanco, en unidades sin escalar */
    draw_text_h(help_text, 140, 55, 0.5f, COL_BLACK, (190 - 20) / 0.5f);
}
void help_exit(void)
{
    if (help_buf)
    {
        C2D_TextBufDelete(help_buf);
        help_buf = NULL;
    }

    help_active = 0;
}