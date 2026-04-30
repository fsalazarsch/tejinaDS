#include "keyboard.h"


static C2D_Font font_kbd;
static C2D_TextBuf buf_kbd;

void kbd_init() {
    font_kbd = C2D_FontLoad("romfs:/MPLUS1p-Medium.bcfnt");
    buf_kbd = C2D_TextBufNew(4096);
}

float* draw_kbdt(float pos_x, float pos_y, u32 color, u32 color2, u32 colorText, C2D_Text* g_staticText, float span, const char *text) {
    float *coords = (float*)malloc(4 * sizeof(float));


    //if (strcmp(text, "←") == 0) { ... }

    // Dimensiones del rectángulo interior (el clickeable)
    float rect_x = pos_x + 1;
    float rect_y = pos_y + 55;
    float rect_w = 27.0f;
    float rect_h = 27.0f;

    if (strcmp(text, " ") == 0) { 
        rect_w *= 5;
    }

    if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        rect_w *= 2;
    }
    DrawRoundedRect(pos_x,     pos_y + 54, rect_w+3, rect_h+3, 5, color);
    DrawRoundedRect(rect_x,    rect_y,     rect_w, rect_h, 5, color2);

    // Medir el texto para centrarlo
    float scaleX = 0.80f;
    float scaleY = 0.9f;
     if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        scaleY = 0.7f;
    }
    float textW, textH;
    C2D_TextGetDimensions(g_staticText, scaleX, scaleY, &textW, &textH);

    // Centrar dentro del rect interior
    float text_x = rect_x + (rect_w - textW) / 2.0f;
    float text_y = rect_y + (rect_h + textH) / 2.0f - 8;

     if ((strcmp(text, "かな") == 0) || (strcmp(text, "カナ") == 0) ) { 
        text_y += 3;
    }
    C2D_DrawText(g_staticText, C2D_AtBaseline | C2D_WithColor, text_x, text_y, 1.0f, scaleX, scaleY, colorText);
    C2D_TextOptimize(g_staticText);

    coords[0] = rect_x;
    coords[1] = rect_y;
    coords[2] = rect_w + coords[0];
    coords[3] = rect_h + coords[1];
    return coords;
}




void kbd_render(C2D_TextBuf g_staticBuf){


    C2D_Text teststring;
    //char *dsplay_kbtabs = "かASDFGHJKL";//poner candado y lupa al final
    
    /*const char *dsplay_kbtabs[5][11] = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "←"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "↵"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", "ñ", "'"},
        {"z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "@"},
        {"⇧", "漢", " ", "", "",  "", "", "かな", NULL, "カナ", NULL}
        
        };  
    const char *dsplay_kbtabs_lowercase[5][11] = {
        {"!", "\"", "#", "$", "%", "&", "(", ")", "=", "?", "←"},
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "↵"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", "Ñ", "["},
        {"Z", "X", "C", "V", "B", "N", "M", ";", ":", "-", "]"},
        {"⇧", "漢", " ", "", "",  "", "", "かな", NULL, "カナ", NULL}
        
        };*/

    const char *dsplay_kbtabs[3][2][5][11] = {

    { // [0] Hiragana (JIS配列)
        { // [0][0] Normal
            {"ぬ", "ふ", "あ", "う", "え", "お", "や", "ゆ", "よ", "わ", "←"},
            {"た", "て", "い", "す", "か", "ん", "な", "に", "ら", "せ", "↵"},
            {"ち", "と", "し", "は", "き", "く", "ま", "の", "り", "れ", "け"},
            {"む", "つ", "さ", "そ", "ひ", "こ", "み", "も", "ね", "る", "め"},
            {"⇧", "ろ", " ",  "",   "",   "",   "",   "ほ", "へ", "カナ", NULL}
        },
        { // [0][1] Shift
            {"",   "ぁ", "ぅ", "ぇ", "ぉ", "ゃ", "ゅ", "ょ", "を", "ー", "←"},
            {"",   "",   "ぃ", "",   "",   "",   "",   "",   "゛", "゜", "↵"},
            {"",   "",   "",   "",   "",   "",   "",   "",   "",   "",   ""  },
            {"",   "っ", "",   "",   "",   "",   "",   "",   "、", "。", "・"},
            {"⇧", "漢", " ",  "",   "",   "",   "",   NULL, NULL, "カナ", NULL}
        }
    },

    { // [1] Katakana (JIS配列)
        { // [1][0] Normal
            {"ヌ", "フ", "ア", "ウ", "エ", "オ", "ヤ", "ユ", "ヨ", "ワ", "←"},
            {"タ", "テ", "イ", "ス", "カ", "ン", "ナ", "ニ", "ラ", "セ", "↵"},
            {"チ", "ト", "シ", "ハ", "キ", "ク", "マ", "ノ", "リ", "レ", "ケ"},
            {"ム", "ツ", "サ", "ソ", "ヒ", "コ", "ミ", "モ", "ネ", "ル", "メ"},
            {"⇧", "ロ", " ",  "",   "",   "",   "",   "ホ", "ヘ", "カナ", NULL}
        },
        { // [1][1] Shift
            {"",   "ァ", "ゥ", "ェ", "ォ", "ャ", "ュ", "ョ", "ヲ", "ー", "←"},
            {"",   "",   "ィ", "",   "",   "",   "",   "",   "゛", "゜", "↵"},
            {"",   "",   "",   "",   "",   "",   "",   "",   "",   "",   ""  },
            {"",   "ッ", "",   "",   "",   "",   "",   "",   "、", "。", "・"},
            {"⇧", "漢", " ",  "",   "",   "",   "",   NULL, NULL, "カナ", NULL}
        }
    },

    { // [2] Romaji
        { // [2][0] Normal
            {"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "0",  "←"},
            {"q",  "w",  "e",  "r",  "t",  "y",  "u",  "i",  "o",  "p",  "↵"},
            {"a",  "s",  "d",  "f",  "g",  "h",  "j",  "k",  "l",  "ñ",  "'"},
            {"z",  "x",  "c",  "v",  "b",  "n",  "m",  ",",  ".",  "/",  "@"},
            {"⇧", "漢", " ",  "",   "",   "",   "",   "かな", NULL, "カナ", NULL}
        },
        { // [2][1] Shift
            {"!",  "\"", "#",  "$",  "%",  "&",  "(",  ")",  "=",  "?",  "←"},
            {"Q",  "W",  "E",  "R",  "T",  "Y",  "U",  "I",  "O",  "P",  "↵"},
            {"A",  "S",  "D",  "F",  "G",  "H",  "J",  "K",  "L",  "Ñ",  "["},
            {"Z",  "X",  "C",  "V",  "B",  "N",  "M",  ";",  ":",  "-",  "]"},
            {"⇧", "漢", " ",  "",   "",   "",   "",   "かな", NULL, "カナ", NULL}
        }
    }

};
    
    
    float **coords_kbd = (float **)malloc(55 * sizeof(float *));
    float span = 1;
    //ESTAS SON LAS COORDENADAS DE LAS TECLAS
    int coordsx[]= { 9, 36, 63, 90, 117, 144, 171, 198, 225, 252, 279 };



    // Variables de estado del teclado
    int kb_mode  = 0; // 0=hiragana, 1=katakana, 2=romaji
    int kb_shift = 0; // 0=normal,   1=shift

    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 11; i++) {
            const char *key = dsplay_kbtabs[kb_mode][kb_shift][j][i];

            if (key == NULL || key[0] == '\0') continue;

            C2D_TextFontParse(&teststring, font_kbd, g_staticBuf, key);
            coords_kbd[i] = draw_kbdt(coordsx[i], 30*(j+1), KANJI_COLOR_BORDER, KANJI_COLOR_BACK, KANJI_COLOR_KEY, &teststring, span, key);
        }
    }


}

void kbd_exit() {
    C2D_TextBufDelete(buf_kbd);
    C2D_FontFree(font_kbd);
}