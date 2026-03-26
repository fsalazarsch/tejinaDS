#ifndef KANJI_STROKE_H
#define KANJI_STROKE_H

#include <citro2d.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ─── Estructuras ─────────────────────────────────────────────────────────────

#define MAX_POINTS_PER_STROKE 128
#define MAX_STROKES_PER_KANJI 32

typedef struct {
    float x, y;
} Point;

typedef struct {
    Point points[MAX_POINTS_PER_STROKE];
    u8    num_points;
} Stroke;

typedef struct {
    Stroke strokes[MAX_STROKES_PER_KANJI];
    u8     num_strokes;
} Kanji;

// ─── Escala KanjiVG (0-109) → pantalla bottom 3DS (320x240) ─────────────────
// Ajusta SIZE, OFFSET_X, OFFSET_Y para posicionar el kanji donde quieras

#define KANJI_SIZE     180.0f
#define KANJI_OFFSET_X 30.0f
#define KANJI_OFFSET_Y 60.0f

#define SCALE_X(x) ((x) * KANJI_SIZE / 109.0f + KANJI_OFFSET_X)
#define SCALE_Y(y) ((y) * KANJI_SIZE / 109.0f + KANJI_OFFSET_Y)

// ─── Colores ─────────────────────────────────────────────────────────────────

#define KANJI_COLOR_CURR C2D_Color32(0,   0,   0,   255)  // negro — stroke actual
#define KANJI_COLOR_DONE C2D_Color32(180, 180, 180, 255)  // gris  — strokes previos

// Grosor de línea en píxeles
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

// ─── Parser de path SVG ──────────────────────────────────────────────────────

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

        if (cmd == 'M' || cmd == 'm') {
            cx = (cmd == 'm') ? cx + args[0] : args[0];
            cy = (cmd == 'm') ? cy + args[1] : args[1];
            if (stroke->num_points < MAX_POINTS_PER_STROKE) {
                stroke->points[stroke->num_points].x = SCALE_X(cx);
                stroke->points[stroke->num_points].y = SCALE_Y(cy);
                stroke->num_points++;
            }

        } else if (cmd == 'L' || cmd == 'l') {
            float tx = (cmd == 'l') ? cx + args[0] : args[0];
            float ty = (cmd == 'l') ? cy + args[1] : args[1];
            if (stroke->num_points < MAX_POINTS_PER_STROKE) {
                stroke->points[stroke->num_points].x = SCALE_X(tx);
                stroke->points[stroke->num_points].y = SCALE_Y(ty);
                stroke->num_points++;
            }
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
                if (stroke->num_points < MAX_POINTS_PER_STROKE) {
                    stroke->points[stroke->num_points].x = SCALE_X(ox);
                    stroke->points[stroke->num_points].y = SCALE_Y(oy);
                    stroke->num_points++;
                }
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
                if (stroke->num_points < MAX_POINTS_PER_STROKE) {
                    stroke->points[stroke->num_points].x = SCALE_X(ox);
                    stroke->points[stroke->num_points].y = SCALE_Y(oy);
                    stroke->num_points++;
                }
            }
            lc2x = p2x; lc2y = p2y;
            cx = p3x;   cy = p3y;
        }
    }
}

// ─── Dibujo interno de un stroke parcial ─────────────────────────────────────

static inline void _draw_stroke_partial(Stroke *stroke, int up_to, u32 color)
{
    for (int i = 0; i < up_to - 1 && i < stroke->num_points - 1; i++) {
        C2D_DrawRectSolid(
            stroke->points[i].x - 2,
            stroke->points[i].y - 2,
            0, 4, 4, color
        );
    }
}

// ─── Dibujo estático ─────────────────────────────────────────────────────────
// Dibuja el kanji completo de una vez
// Llamar dentro de C2D_SceneBegin(bottom)

static inline void draw_kanji_static(Kanji *kanji)
{
    for (int s = 0; s < kanji->num_strokes; s++)
        _draw_stroke_partial(&kanji->strokes[s],
                              kanji->strokes[s].num_points,
                              KANJI_COLOR_CURR);
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

// ─── Animación (stroke por stroke progresivo) ────────────────────────────────
// Llamar UNA VEZ POR FRAME dentro de C2D_SceneBegin(bottom)
// Retorna 1 cuando terminó, 0 si sigue

static inline int animate_kanji(Kanji *kanji, KanjiAnimState *state)
{
    if (state->stroke >= kanji->num_strokes) {
        draw_kanji_static(kanji);
        return 1;
    }
    // Strokes anteriores completos en gris
    for (int s = 0; s < state->stroke; s++)
        _draw_stroke_partial(&kanji->strokes[s],
                              kanji->strokes[s].num_points,
                              KANJI_COLOR_DONE);

    // Stroke actual parcial en negro
    _draw_stroke_partial(&kanji->strokes[state->stroke],
                          state->point,
                          KANJI_COLOR_CURR);

    // Avanzar estado
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

#endif // KANJI_STROKE_H