#include "keyboard.h"

static C2D_Font font_kbd;
static C2D_TextBuf buf_kbd;

   const char *dsplay_kbtabs[3][6][10] = {
    
    { // Hiragana (JIS配列)
        {"あ", "か", "さ", "た", "な", "は", "ま", "や", "ら", "わ"},
        {"い", "き", "し", "ち", "に", "ひ", "み", "ゆ", "り", "ん"},
        {"う", "く", "す", "つ", "ぬ", "ふ", "む", "よ", "る", "を"},
        {"え", "け", "せ", "て", "ね", "へ", "め", "゛", "れ", "ー"},
        {"お", "こ", "そ", "と", "の", "ほ", "も", "゜",  "ろ", "小"},
        {"㋺", "カナ", NULL," ",  "",   "",   "",  "",   "←", "↵"}
    },
    { // [1][0] Normal
        {"ア", "カ", "サ", "タ", "ナ", "ハ", "マ", "ヤ", "ラ", "ワ"},
        {"イ", "キ", "シ", "チ", "ニ", "ヒ", "ミ", "ユ", "リ", "ン"},
        {"ウ", "ク", "ス", "ツ", "ヌ", "フ", "ム", "ヨ", "ル", "ヲ"},
        {"エ", "ケ", "セ", "テ", "ネ", "ヘ", "メ", "゛", "レ", "ー"},
        {"オ", "コ", "ソ", "ト", "ノ", "ホ", "モ", "゜", "ロ", "小"},
        {"㋺", "かな", NULL," ",  "",   "",   "", "", "←", "↵"}
    },
        { // [2][0] Normal
        {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        {"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "0"},
        {"q",  "w",  "e",  "r",  "t",  "y",  "u",  "i",  "o",  "p"},
        {"a",  "s",  "d",  "f",  "g",  "h",  "j",  "k",  "l",  "ñ"},
        {"z",  "x",  "c",  "v",  "b",  "n",  "m",  ",",  "←", "↵"},
        {".", " ", "",  "",   "",   "",   "",   "かな", "カナ", NULL}
    }
};


void    kbd_init() {
    font_kbd = C2D_FontLoad("romfs:/MPLUS1p-Medium.bcfnt");
    buf_kbd = C2D_TextBufNew(4096);
}

void draw_kbdt(float pos_x, float pos_y, u32 color, u32 color2, u32 colorText, C2D_Text* g_staticText, float span, const char *text, float* out_coords) {
    // Dimensiones del rectángulo interior
    float rect_x = pos_x + 1;
    float rect_y = pos_y + 55;
    float rect_w = 24.0f;
    float rect_h = 24.0f;

    if (strcmp(text, " ") == 0) { 
        rect_w *= 5;
    }

    if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        rect_w *= 2;
    }
    DrawRoundedRect(pos_x,     pos_y + 54, rect_w+3, rect_h+3, 5, color);
    DrawRoundedRect(rect_x,    rect_y,     rect_w, rect_h, 5, color2);

    float scaleX = 0.80f;
    float scaleY = 0.9f;
    if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        scaleY = 0.7f;
    }
    float textW, textH;
    C2D_TextGetDimensions(g_staticText, scaleX, scaleY, &textW, &textH);

    float text_x = rect_x + (rect_w - textW) / 2.0f;
    float text_y = rect_y + (rect_h + textH) / 2.0f - 8;

    if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        text_y += 3;
    }
    C2D_DrawText(g_staticText, C2D_AtBaseline | C2D_WithColor, text_x, text_y, 1.0f, scaleX, scaleY, colorText);
    C2D_TextOptimize(g_staticText);

    // Guardar directamente en el espacio de memoria que pasamos por parámetro
    out_coords[0] = rect_x;
    out_coords[1] = rect_y;
    out_coords[2] = rect_w + rect_x;
    out_coords[3] = rect_h + rect_y;
}

void kbd_render(C2D_TextBuf g_staticBuf, Keyboard *kbd, int kbmode){
    kbd->key_count = 0;
    kbd->kb_mode = kbmode;
    C2D_Text teststring;
    
    int coordsx[]= { 19, 46, 73, 100, 127, 154, 181, 208, 235, 262 };
    float span = 1;

    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 10; i++) {
            const char *key = dsplay_kbtabs[kbd->kb_mode][j][i];

            if (key == NULL || key[0] == '\0') continue;

            int idx = kbd->key_count;

            C2D_TextFontParse(&teststring, font_kbd, g_staticBuf, key);
            
            // Pasamos directamente la dirección de kbd->key_coords[idx]
            draw_kbdt(coordsx[i], 25*(j+1), KANJI_COLOR_BORDER, KANJI_COLOR_BACK, KANJI_COLOR_KEY, &teststring, span, key, kbd->key_coords[idx]);
            
            strncpy(kbd->key_chars[idx], key, 7);
            kbd->key_chars[idx][7] = '\0';
            kbd->key_count++;
        }
    }
}


const char* kbd_update(Keyboard *kbd, touchPosition *touch, u32 kDown, u32 kHeld, u32 kUp) {
    if (!(kDown & KEY_TOUCH))
        return NULL;  // no hubo toque

    float tx = (float)touch->px;
    float ty = (float)touch->py;

    for (int i = 0; i < kbd->key_count; i++) {
        if (tx >= kbd->key_coords[i][0] && tx <= kbd->key_coords[i][2] &&
            ty >= kbd->key_coords[i][1] && ty <= kbd->key_coords[i][3]) {

            // Teclas especiales

            if (strcmp(kbd->key_chars[i], "かな") == 0) {
                kbd->kb_mode = 0;    // hiragana      
                //kbd_render(g_staticBuf, kbd);      
                return NULL;
            }
            if (strcmp(kbd->key_chars[i], "カナ") == 0) {
                kbd->kb_mode = 1;    // katakana
                return NULL;
            }
            if (strcmp(kbd->key_chars[i], "㋺") == 0) {
                kbd->kb_mode = 2;    // romaji
                return NULL;
            }

            // Tecla normal — resetear shift después de presionar
            //if (kbd->kb_shift) kbd->kb_shift = 0;

            return kbd->key_chars[i];  // ← el caracter presionado
        }
    }

    return NULL;  // toque fuera del teclado
}


void kbd_exit() {
    C2D_TextBufDelete(buf_kbd);
    C2D_FontFree(font_kbd);
}