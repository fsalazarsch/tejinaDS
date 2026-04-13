#ifndef DRAWING_H
#define DRAWING_H

#include <citro2d.h>
#include <3ds.h>

#define MAX_DRAW_POINTS 512

typedef struct { int x, y; int stroke; } DrawPoint;

static DrawPoint drawPoints[MAX_DRAW_POINTS];
static int drawCount = 0;
static int currentStroke = 0;

static inline void drawing_clear(void) {
    drawCount = 0;
    currentStroke = 0;
}

static inline void drawing_update(u32 kHeld, u32 kUp, int tx, int ty,
                                   int ax, int ay, int aw, int ah) {
    if (kUp & KEY_TOUCH)
        currentStroke++;

    if (!(kHeld & KEY_TOUCH)) return;
    if (!isTouchInRect(tx, ty, ax, ay, aw, ah)) return;
    if (drawCount < MAX_DRAW_POINTS) {
        drawPoints[drawCount].x = tx;
        drawPoints[drawCount].y = ty;
        drawPoints[drawCount].stroke = currentStroke;
        drawCount++;
    }
}

static inline void drawing_draw(u32 color) {
    for (int i = 1; i < drawCount; i++) {
        if (drawPoints[i].stroke != drawPoints[i-1].stroke) continue;
        int x0 = drawPoints[i-1].x, y0 = drawPoints[i-1].y;
        int x1 = drawPoints[i].x,   y1 = drawPoints[i].y;
        int dx = x1-x0, dy = y1-y0;
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        if (steps == 0) {
            C2D_DrawRectSolid(x0-2, y0-2, 0.3f, 4, 4, color);
            continue;
        }
        for (int s = 0; s <= steps; s++) {
            float t = (float)s / steps;
            C2D_DrawRectSolid((int)(x0+dx*t)-2, (int)(y0+dy*t)-2, 0.3f, 4, 4, color);
        }
    }
}

#endif