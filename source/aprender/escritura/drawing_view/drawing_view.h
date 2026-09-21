#ifndef DRAWING_VIEW_H
#define DRAWING_VIEW_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "../../../functions.h"

/* Resultados de drawing_view_touch */
#define DRAW_VIEW_TOUCH_NONE  0
#define DRAW_VIEW_TOUCH_CLEAR 1
#define DRAW_VIEW_TOUCH_HELP  2

/* Colores del tema (evita dependencia directa con el tema actual) */
typedef struct {
    u32 cellIdle;
    u32 borderCell;
    u32 kanaText;
    u32 btnAudio;
    u32 btnAudioText;
} DrawingViewColors;

/* Botón adosado a la vista (la única "variable" entre pantallas) */
typedef struct {
    int x, y, w, h;
    int id;             /* se devuelve al tocarlo (>= 3) */
    const char* label;  /* texto actual del botón */
    int active;         /* != 0 => fondo btnAudio (resaltado) */
    int accent;         /* != 0 => texto en btnAudioText */
} DrawingViewButton;

/* Geometría de la vista: panel cuadriculado + X/? + botones */
typedef struct {
    int x, y, w, h;        /* rect del cuadriculado */
    int vdiv1;             /* divisor vertical (-1 ninguno) */
    int vdiv2;             /* segundo divisor vertical (-1 ninguno) */
    int hdiv;              /* divisor horizontal (-1 ninguno) */

    int clear_x, clear_y;  /* botón X (30x30) */
    int help_x, help_y;    /* botón ? (30x30) */

    const DrawingViewButton* buttons;
    int n_buttons;
} DrawingViewGeom;

void drawing_view_clear(void);

/* stylus: llamar cada frame mientras se interactúa con el panel */
void drawing_view_update(u32 kHeld, u32 kUp, int tx, int ty,
                         const DrawingViewGeom* g);

/* toque: devuelve DRAW_VIEW_TOUCH_* o el id de un botón */
int  drawing_view_touch(int tx, int ty, const DrawingViewGeom* g);

/* fase 1: panel cuadriculado (con divisores) */
void drawing_view_render_panel(const DrawingViewGeom* g,
                               DrawingViewColors c);

/* fase 2: canvas (trazos del stylus) + X/? + botones.
   Dibujar el contenido del usuario entre panel y tail. */
void drawing_view_render_tail(C2D_TextBuf staticBuf, C2D_Font font2,
                              const DrawingViewGeom* g,
                              DrawingViewColors c);

/* panel + tail de una vez (vista sin contenido intermedio) */
void drawing_view_render(C2D_TextBuf staticBuf, C2D_Font font2,
                         const DrawingViewGeom* g,
                         DrawingViewColors c);

#endif