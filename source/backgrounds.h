#define SCREEN_WIDTH_DS 256
#define SCREEN_HEIGHT_DS 192

#define clrBlack  RGB15(0,  0,  0) | BIT(15)
#define clrWhite  RGB15(31, 31, 31) | BIT(15)
#define clrRed    RGB15(31, 0,  0) | BIT(15)
#define clrGreen  RGB15(0,  20, 6) | BIT(15)
#define clrBlue   RGB15(0,  0,  31) | BIT(15)
#define clrLightBlue RGB15(19, 25, 31) | BIT(15)


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
                 u16 color, u16 borderColor,
                 dsf_handle font, const void *bitmap, int bmp_w, int bmp_h,
                 const char *text, int text_x, int text_y)
{

    void *out_texture;
    size_t out_width, out_height;

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
            *dst++ = *src++;
    }

    //draw_void_rect(fb, x1, y1, x2, y2, border, borderColor);
    draw_void_rect(fb, x1, y1, w, h, border, borderColor);

    free(out_texture);
}

void draw_menu(int bg_sub,
               dsf_handle font,
               const void *bitmap,
               const char *options[],
               int total_options)
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
                    clrLightBlue, clrBlue,
                    font, bitmap, 256, 256,
                    options[i], x + 10, y + 5);

        y += button_height + spacing;
    }
}

/*void draw_menu(int bg_sub, dsf_handle font, const void *bitmap ){
    uint16_t *fb = bgGetGfxPtr(bg_sub);
    

    int margin_top = 10;
    int spacing = 10; // espacio entre botones
    int button_height = 35;
    int button_width = SCREEN_WIDTH_DS - 20;
    int x = 10;

    int y = margin_top;

    // Botón 1
    draw_button(fb, x, y, button_width, button_height, 2,
                clrLightBlue, clrBlue,
                font, bitmap, 256, 256,
                "(Y) Vocabulario inicial", x + 10, y + 5);

    y += button_height + spacing;

    // Botón 2
    draw_button(fb, x, y, button_width, button_height, 2,
                clrLightBlue, clrBlue,
                font, bitmap, 256, 256,
                "(X) Vocabulario intermedio", x + 10, y + 5);

    y += button_height + spacing;

    // Botón 3
    draw_button(fb, x, y, button_width, button_height, 2,
                clrLightBlue, clrBlue,
                font, bitmap, 256, 256,
                "(A) Kanji básicos", x + 10, y + 5);

    y += button_height + spacing;

    // Botón 4
    draw_button(fb, x, y, button_width, button_height, 2,
                clrLightBlue, clrBlue,
                font, bitmap, 256, 256,
                "(B) Configuración", x + 10, y + 5);

}*/