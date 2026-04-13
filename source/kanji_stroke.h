#ifndef KANJI_STROKE_H
#define KANJI_STROKE_H

#include <citro2d.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "themes.h"

extern ThemeID currentTheme;


// ─── Estructuras ─────────────────────────────────────────────────────────────

#define MAX_POINTS_PER_STROKE 256
#define MAX_STROKES_PER_KANJI 32

typedef struct {
    float x, y;  // coordenadas raw KanjiVG (0-109)
} Point;

typedef struct {
    Point points[MAX_POINTS_PER_STROKE];
    u8    num_points;
} Stroke;

typedef struct {
    Stroke strokes[MAX_STROKES_PER_KANJI];
    u8     num_strokes;
} Kanji;

// ─── Colores ─────────────────────────────────────────────────────────────────

#define KANJI_COLOR_CURR themes[currentTheme].accent
#define KANJI_COLOR_DONE themes[currentTheme].hintText
#define KANJI_LINE_WIDTH 3.0f

// ─── Bézier cúbica ───────────────────────────────────────────────────────────

static inline void _cubic_bezier(float p0x, float p0y,
                                   float p1x, float p1y,
                                   float p2x, float p2y,
                                   float p3x, float p3y,
                                   float t, float *ox, float *oy)
{
    float u  = 1.0f - t;
    float u2 = u * u;
    float u3 = u2 * u;
    float t2 = t * t;
    float t3 = t2 * t;
    *ox = u3*p0x + 3*u2*t*p1x + 3*u*t2*p2x + t3*p3x;
    *oy = u3*p0y + 3*u2*t*p1y + 3*u*t2*p2y + t3*p3y;
}

// ─── Parser de path SVG (guarda coords raw 0-109) ────────────────────────────

static inline void parse_svg_path(const char *path_data, Stroke *stroke)
{
    stroke->num_points = 0;

    float cx = 0, cy = 0;
    float lc2x = 0, lc2y = 0;
    char  cmd  = 0;
    const char *p = path_data;
    const int steps = 20;

    while (*p) {
        while (*p == ' ' || *p == ',' || *p == '\t') p++;
        if (!*p) break;

        if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z'))
            cmd = *p++;

        float args[8];
        int nargs = 0;
        while (nargs < 8) {
            while (*p == ' ' || *p == ',') p++;
            if (!*p || ((*p < '0' || *p > '9') && *p != '-' && *p != '.')) break;
            char *end;
            args[nargs++] = strtof(p, &end);
            p = end;
        }

        #define PUSH(px, py) \
            if (stroke->num_points < MAX_POINTS_PER_STROKE) { \
                stroke->points[stroke->num_points].x = (px); \
                stroke->points[stroke->num_points].y = (py); \
                stroke->num_points++; \
            }

        if (cmd == 'M' || cmd == 'm') {
            cx = (cmd == 'm') ? cx + args[0] : args[0];
            cy = (cmd == 'm') ? cy + args[1] : args[1];
            PUSH(cx, cy);

        } else if (cmd == 'L' || cmd == 'l') {
            float tx = (cmd == 'l') ? cx + args[0] : args[0];
            float ty = (cmd == 'l') ? cy + args[1] : args[1];
            PUSH(tx, ty);
            cx = tx; cy = ty;

        } else if (cmd == 'C' || cmd == 'c') {
            float p1x = (cmd == 'c') ? cx + args[0] : args[0];
            float p1y = (cmd == 'c') ? cy + args[1] : args[1];
            float p2x = (cmd == 'c') ? cx + args[2] : args[2];
            float p2y = (cmd == 'c') ? cy + args[3] : args[3];
            float p3x = (cmd == 'c') ? cx + args[4] : args[4];
            float p3y = (cmd == 'c') ? cy + args[5] : args[5];
            for (int i = 1; i <= steps; i++) {
                float t = (float)i / steps;
                float ox, oy;
                _cubic_bezier(cx, cy, p1x, p1y, p2x, p2y, p3x, p3y, t, &ox, &oy);
                PUSH(ox, oy);
            }
            lc2x = p2x; lc2y = p2y;
            cx = p3x;   cy = p3y;

        } else if (cmd == 'S' || cmd == 's') {
            float p1x = 2*cx - lc2x;
            float p1y = 2*cy - lc2y;
            float p2x = (cmd == 's') ? cx + args[0] : args[0];
            float p2y = (cmd == 's') ? cy + args[1] : args[1];
            float p3x = (cmd == 's') ? cx + args[2] : args[2];
            float p3y = (cmd == 's') ? cy + args[3] : args[3];
            for (int i = 1; i <= steps; i++) {
                float t = (float)i / steps;
                float ox, oy;
                _cubic_bezier(cx, cy, p1x, p1y, p2x, p2y, p3x, p3y, t, &ox, &oy);
                PUSH(ox, oy);
            }
            lc2x = p2x; lc2y = p2y;
            cx = p3x;   cy = p3y;
        }

        #undef PUSH
    }
}

// ─── Dibujo con offset y size dinámicos ──────────────────────────────────────

static inline void _draw_stroke_partial_at(Stroke *stroke, int up_to, u32 color,
                                            float ox, float oy, float size)
{
    float scale = size / 109.0f;
    for (int i = 0; i < up_to - 1 && i < stroke->num_points - 1; i++) {
        float x0 = stroke->points[i].x   * scale + ox;
        float y0 = stroke->points[i].y   * scale + oy;
        float x1 = stroke->points[i+1].x * scale + ox;
        float y1 = stroke->points[i+1].y * scale + oy;
        C2D_DrawRectSolid(x0 - 2, y0 - 2, 0, 4, 4, color);
        (void)x1; (void)y1; // usar cuando C2D_DrawLine funcione
    }
}

// ─── Dibujo estático ─────────────────────────────────────────────────────────

static inline void draw_kanji_static_at(Kanji *kanji, float x, float y, float size)
{
    for (int s = 0; s < kanji->num_strokes; s++)
        _draw_stroke_partial_at(&kanji->strokes[s],
                                 kanji->strokes[s].num_points,
                                 KANJI_COLOR_CURR, x, y, size);
}

// Versión con defaults (compatibilidad)
static inline void draw_kanji_static(Kanji *kanji)
{
    draw_kanji_static_at(kanji, 80.0f, 40.0f, 160.0f);
}

// ─── Estado de animación ─────────────────────────────────────────────────────

typedef struct {
    int stroke;
    int point;
    int delay;
} KanjiAnimState;

static inline void kanji_anim_init(KanjiAnimState *state)
{
    state->stroke = 0;
    state->point  = 2;
    state->delay  = 0;
}

// ─── Animación ───────────────────────────────────────────────────────────────

static inline int animate_kanji_at(Kanji *kanji, KanjiAnimState *state,
                                    float x, float y, float size)
{
    if (state->stroke >= kanji->num_strokes) {
        draw_kanji_static_at(kanji, x, y, size);
        return 1;
    }

    for (int s = 0; s < state->stroke; s++)
        _draw_stroke_partial_at(&kanji->strokes[s],
                                 kanji->strokes[s].num_points,
                                 KANJI_COLOR_CURR, x, y, size);

    _draw_stroke_partial_at(&kanji->strokes[state->stroke],
                             state->point,
                             KANJI_COLOR_DONE, x, y, size);

    if (state->delay > 0) {
        state->delay--;
    } else {
        state->point++;
        if (state->point > kanji->strokes[state->stroke].num_points) {
            state->stroke++;
            state->point = 2;
            state->delay = 20;
        }
    }

    return 0;
}

// Versión con defaults (compatibilidad)
static inline int animate_kanji(Kanji *kanji, KanjiAnimState *state)
{
    return animate_kanji_at(kanji, state, 80.0f, 40.0f, 160.0f);
}

#endif // KANJI_STROKE_H