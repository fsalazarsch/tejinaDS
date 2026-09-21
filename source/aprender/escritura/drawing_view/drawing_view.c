/*
 * =========================================================
 *   drawing_view.c  —  vista reutilizable de tablero de
 *                      escritura (panel cuadriculado + stylus
 *                      + botones adosados X/?/personalizados)
 *
 *   El único cambio entre pantallas (DIBUJAR, BUSCAR, kana)
 *   son los botones y la geometría del panel.
 * =========================================================
 */

#include "drawing_view.h"
#include "../../../drawing.h"

void drawing_view_clear(void)
{
    drawing_clear();
}

void drawing_view_update(u32 kHeld, u32 kUp, int tx, int ty,
                         const DrawingViewGeom* g)
{
    drawing_update(kHeld, kUp, tx, ty, g->x, g->y, g->w, g->h);
}

int drawing_view_touch(int tx, int ty, const DrawingViewGeom* g)
{
    if (isTouchInRect(tx, ty, g->clear_x, g->clear_y, 30, 30))
        return DRAW_VIEW_TOUCH_CLEAR;

    if (isTouchInRect(tx, ty, g->help_x, g->help_y, 30, 30))
        return DRAW_VIEW_TOUCH_HELP;

    for (int i = 0; i < g->n_buttons; i++) {
        const DrawingViewButton* b = &g->buttons[i];
        if (isTouchInRect(tx, ty, b->x, b->y, b->w, b->h))
            return b->id;
    }

    return DRAW_VIEW_TOUCH_NONE;
}

void drawing_view_render_panel(const DrawingViewGeom* g, DrawingViewColors c)
{
    DrawRoundedRect(g->x, g->y, g->w, g->h, 4, c.cellIdle);
    DrawRoundedRect(g->x, g->y, g->w, g->h, 4, 0x00000000);

    if (g->vdiv1 >= 0) DrawRoundedRect(g->vdiv1, g->y, 2, g->h, 0, c.borderCell);
    if (g->vdiv2 >= 0) DrawRoundedRect(g->vdiv2, g->y, 2, g->h, 0, c.borderCell);
    if (g->hdiv  >= 0) DrawRoundedRect(g->x, g->hdiv, g->w, 2, 0, c.borderCell);
}

static void draw_chrome_button(C2D_TextBuf staticBuf, C2D_Font font2,
                               int x, int y, int w, int h,
                               const char* label, int active, int accent,
                               DrawingViewColors c)
{
    u32 bg = active ? c.btnAudio : c.cellIdle;
    u32 tc = accent ? c.btnAudioText : c.kanaText;
    float scale = (h >= 30) ? 0.8f : 0.7f;

    DrawRoundedRect(x, y, w, h, 8, bg);

    C2D_Text t;
    C2D_TextFontParse(&t, font2, staticBuf, label);
    C2D_TextOptimize(&t);
    C2D_DrawText(&t, C2D_AtBaseline | C2D_AlignCenter,
                 x + w * 0.5f, y + h * 0.5f + 7.0f,
                 0.5f, scale, scale, tc);
}

void drawing_view_render_tail(C2D_TextBuf staticBuf, C2D_Font font2,
                              const DrawingViewGeom* g,
                              DrawingViewColors c)
{
    drawing_draw(c.kanaText);

    /* X y ? */
    draw_chrome_button(staticBuf, font2, g->clear_x, g->clear_y, 30, 30,
                       "X", 0, 0, c);
    draw_chrome_button(staticBuf, font2, g->help_x, g->help_y, 30, 30,
                       "?", 0, 0, c);

    /* botones personalizados */
    for (int i = 0; i < g->n_buttons; i++) {
        const DrawingViewButton* b = &g->buttons[i];
        draw_chrome_button(staticBuf, font2, b->x, b->y, b->w, b->h,
                           b->label, b->active, b->accent, c);
    }
}

void drawing_view_render(C2D_TextBuf staticBuf, C2D_Font font2,
                         const DrawingViewGeom* g,
                         DrawingViewColors c)
{
    drawing_view_render_panel(g, c);
    drawing_view_render_tail(staticBuf, font2, g, c);
}