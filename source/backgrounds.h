#include "themes.h"

#define SCREEN_WIDTH_DS 256
#define SCREEN_HEIGHT_DS 192

#define clrBlack  RGB15(0,  0,  0) | BIT(15)
#define clrWhite  RGB15(31, 31, 31) | BIT(15)
#define clrRed    RGB15(31, 0,  0) | BIT(15)
#define clrGreen  RGB15(0,  20, 6) | BIT(15)
#define clrBlue   RGB15(0,  0,  31) | BIT(15)
#define clrLightBlue RGB15(19, 25, 31) | BIT(15)

typedef struct {
    int x, y;
    int w, h;
} Button;

// Detección de toque sobre botón
static inline int button_hit(Button *btn, touchPosition *touch) {
    return (touch->px >= btn->x && touch->px <= btn->x + btn->w &&
            touch->py >= btn->y && touch->py <= btn->y + btn->h);
}

void wait_forever(void);

void drawPixel(u16 *fb, int x, int y, u16 color)
{
    fb[y * 256 + x] = color;
}

void drawLine(u16 *fb, int x1, int y1, int x2, int y2, u16 color)
{
    if (y1 == y2) // horizontal
        for (int x = x1; x < x2; x++)
            drawPixel(fb, x, y1, color);
    else // vertical
        for (int y = y1; y < y2; y++)
            drawPixel(fb, x1, y, color);
}

void drawRect(u16 *fb, int x1, int x2, int y1, int y2, int border, u16 color, u16 borderColor){
    for (int y = y1; y < y2; y++){
        for (int x = x1; x < x2; x++) {
            if (y < y1 + border || y >= y2 - border ||
                x < x1 + border || x >= x2 - border)
                fb[y * 256 + x] = borderColor;
            else
                fb[y * 256 + x] = color;
        }
    }
}

void draw_void_rect(uint16_t *fb, int x1, int y1, int w, int h, int border, u16 borderColor)
{
    int x2 = x1 + w;
    int y2 = y1 + h;

    for (int i = 0; i < border; i++) {
        drawLine(fb, x1+i, y1+i, x2-i, y1+i, borderColor); // top
        drawLine(fb, x1+i, y2-i-1, x2-i, y2-i-1, borderColor); // bottom
        drawLine(fb, x1+i, y1+i, x1+i, y2-i, borderColor); // left
        drawLine(fb, x2-i-1, y1+i, x2-i-1, y2-i, borderColor); // right
    }
}


void draw_button(uint16_t *fb, int x1, int y1, int w, int h, int border,
                 dsf_handle font, const void *bitmap, int bmp_w, int bmp_h,
                 const char *text, int text_x, int text_y, KanaTheme theme, bool hit)
{


    void *out_texture;
    size_t out_width, out_height;

    u16 color_fondo_boton, color_texto_boton, color_borde_boton; 


    if (hit == 1){
        color_fondo_boton = theme.cellCursor;   // #e8d5a8
        color_borde_boton = theme.accent;       // #8b5e2a
        color_texto_boton = theme.kanaText;     // #2a1a08
    }
    else {
        color_fondo_boton = theme.cellIdle;     // #efe0bc
        color_borde_boton = theme.borderCell;   // #c4a870
        color_texto_boton = theme.headerText;   // #4a3010
    }

    drawRect(fb, x1, x1 + w, y1, y1 + h, border, color_fondo_boton, color_borde_boton);

    dsf_error r = DSF_StringRenderToTexture(font, text, GL_RGBA,
                        bitmap, bmp_w, bmp_h,
                        &out_texture, &out_width, &out_height);
    if (r != DSF_NO_ERROR)
    {
        printf("draw_button() failed: %d", r);
        wait_forever();
    }

    for (int j = 0; j < out_height; j++)
    {
        const uint16_t *src = out_texture;
        uint16_t *dst = fb;
        dst += (j + text_y) * 256 + text_x;
        src += j * out_width;
        for (int i = 0; i < out_width; i++)
        {
            uint16_t pixel = *src++;
            if (pixel & 1)
                *dst++ = color_texto_boton;
            else
                dst++;
        }
    }

    //draw_void_rect(fb, x1, y1, x2, y2, border, borderColor);
    //draw_void_rect(fb, x1, y1, w, h, border, color_borde_boton);

    free(out_texture);
}

void draw_menu(int bg_sub,
               dsf_handle font,
               const void *bitmap,
               const char *options[],
               int total_options, KanaTheme theme)
{
    uint16_t *fb = bgGetGfxPtr(bg_sub);

    int margin_top = 10;
    int spacing = 10;
    int button_height = 35;
    int button_width = SCREEN_WIDTH_DS - 20;
    int x = 10;
    int y = margin_top;

    for (int i = 0; i < total_options; i++)
    {
        draw_button(fb, x, y, button_width, button_height, 2,
                    font, bitmap, 256, 256,
                    options[i], x + 10, y, theme, 0 );

        y += button_height + spacing;
    }
}