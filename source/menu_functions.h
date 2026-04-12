#include "tabla.h"
#include "themes.h"
#include "kanji_data.h"
#include "tts_engine.h"

Kanji k;
KanjiAnimState anim;
Kanji k2;
KanjiAnimState anim2;

extern ThemeID currentTheme;
bool showHelp = false;
bool kanaOculto = false;
bool mostrandoTrazos = false;

const KanaEntry hiragana[5][10] = {
    {
        KANA1("あ","a",0x3042), KANA1("か","ka",0x304B), KANA1("さ","sa",0x3055), KANA1("た","ta",0x305F), KANA1("な","na",0x306A),
        KANA1("は","ha",0x306F), KANA1("ま","ma",0x307E), KANA1("や","ya",0x3084), KANA1("ら","ra",0x3089), KANA1("わ","wa",0x308F)
    },
    {
        KANA1("い","i",0x3044), KANA1("き","ki",0x304D), KANA1("し","shi",0x3057), KANA1("ち","chi",0x3061), KANA1("に","ni",0x306B),
        KANA1("ひ","hi",0x3072), KANA1("み","mi",0x307F), {" "," ",{0,0},0}, KANA1("り","ri",0x308A), {" "," ",{0,0},0}
    },
    {
        KANA1("う","u",0x3046), KANA1("く","ku",0x304F), KANA1("す","su",0x3059), KANA1("つ","tsu",0x3064), KANA1("ぬ","nu",0x306C),
        KANA1("ふ","fu",0x3075), KANA1("む","mu",0x3080), KANA1("ゆ","yu",0x3086), KANA1("る","ru",0x308B), KANA1("ん","n",0x3093)
    },
    {
        KANA1("え","e",0x3048), KANA1("け","ke",0x3051), KANA1("せ","se",0x305B), KANA1("て","te",0x3066), KANA1("ね","ne",0x306D),
        KANA1("へ","he",0x3078), KANA1("め","me",0x3081), {" "," ",{0,0},0}, KANA1("れ","re",0x308C), {" "," ",{0,0},0}
    },
    {
        KANA1("お","o",0x304A), KANA1("こ","ko",0x3053), KANA1("そ","so",0x305D), KANA1("と","to",0x3068), KANA1("の","no",0x306E),
        KANA1("ほ","ho",0x307B), KANA1("も","mo",0x3082), KANA1("よ","yo",0x3088), KANA1("ろ","ro",0x308D), KANA1("を","(w)o",0x3092)
    }
};

const KanaEntry hiragana_dakuten[5][5] = {
    {
        KANA1("が","ga",0x304C), KANA1("ざ","za",0x3056), KANA1("だ","da",0x3060), KANA1("ば","ba",0x3070), KANA1("ぱ","pa",0x3071)
    },
    {
        KANA1("ぎ","gi",0x304E), KANA1("じ","ji",0x3058), KANA1("ぢ","di",0x3062), KANA1("び","bi",0x3073), KANA1("ぴ","pi",0x3074)
    },
    {
        KANA1("ぐ","gu",0x3050), KANA1("ず","zu",0x305A), KANA1("づ","du",0x3065), KANA1("ぶ","bu",0x3076), KANA1("ぷ","pu",0x3077)
    },
    {
        KANA1("げ","ge",0x3052), KANA1("ぜ","ze",0x305C), KANA1("で","de",0x3067), KANA1("べ","be",0x3079), KANA1("ぺ","pe",0x307A)
    },
    {
        KANA1("ご","go",0x3054), KANA1("ぞ","zo",0x305E), KANA1("ど","do",0x3069), KANA1("ぼ","bo",0x307C), KANA1("ぽ","po",0x307D)
    }
};

const KanaEntry hiragana_comb[6][6] = {
    {
        KANA2("きゃ","kya",0x304D,0x3083), KANA2("きゅ","kyu",0x304D,0x3085), KANA2("きょ","kyo",0x304D,0x3087),
        KANA2("ぎゃ","gya",0x304E,0x3083), KANA2("ぎゅ","gyu",0x304E,0x3085), KANA2("ぎょ","gyo",0x304E,0x3087)
    },
    {
        KANA2("しゃ","sha",0x3057,0x3083), KANA2("しゅ","shu",0x3057,0x3085), KANA2("しょ","sho",0x3057,0x3087),
        KANA2("じゃ","ja",0x3058,0x3083),  KANA2("じゅ","ju",0x3058,0x3085),  KANA2("じょ","jo",0x3058,0x3087)
    },
    {
        KANA2("ちゃ","cha",0x3061,0x3083), KANA2("ちゅ","chu",0x3061,0x3085), KANA2("ちょ","cho",0x3061,0x3087),
        KANA2("りゃ","rya",0x308A,0x3083), KANA2("りゅ","ryu",0x308A,0x3085), KANA2("りょ","ryo",0x308A,0x3087)
    },
    {
        KANA2("にゃ","nya",0x306B,0x3083), KANA2("にゅ","nyu",0x306B,0x3085), KANA2("にょ","nyo",0x306B,0x3087),
        KANA2("みゃ","mya",0x307F,0x3083), KANA2("みゅ","myu",0x307F,0x3085), KANA2("みょ","myo",0x307F,0x3087)
    },
    {
        KANA2("ひゃ","hya",0x3072,0x3083), KANA2("ひゅ","hyu",0x3072,0x3085), KANA2("ひょ","hyo",0x3072,0x3087),
        KANA2("びゃ","bya",0x3073,0x3083), KANA2("びゅ","byu",0x3073,0x3085), KANA2("びょ","byo",0x3073,0x3087)
    },
    {
        KANA2("ぴゃ","pya",0x3074,0x3083), KANA2("ぴゅ","pyu",0x3074,0x3085), KANA2("ぴょ","pyo",0x3074,0x3087),        
        KANA2("※ゃ","Comparación ya",0x203B,0x3083), KANA2("※ゅ","Comparación yu",0x203B,0x3085), KANA2("※ょ","Comparación yo",0x203B,0x3087)
    }
};

const KanaEntry katakana[5][10] = {
    {
        KANA1("ア","a",0x30A2), KANA1("カ","ka",0x30AB), KANA1("サ","sa",0x30B5), KANA1("タ","ta",0x30BF), KANA1("ナ","na",0x30CA),
        KANA1("ハ","ha",0x30CF), KANA1("マ","ma",0x30DE), KANA1("ヤ","ya",0x30E4), KANA1("ラ","ra",0x30E9), KANA1("ワ","wa",0x30EF)
    },
    {
        KANA1("イ","i",0x30A4), KANA1("キ","ki",0x30AD), KANA1("シ","shi",0x30B7), KANA1("チ","chi",0x30C1), KANA1("ニ","ni",0x30CB),
        KANA1("ヒ","hi",0x30D2), KANA1("ミ","mi",0x30DF), {" "," ",{0,0},0}, KANA1("リ","ri",0x30EA), {" "," ",{0,0},0}
    },
    {
        KANA1("ウ","u",0x30A6), KANA1("ク","ku",0x30AF), KANA1("ス","su",0x30B9), KANA1("ツ","tsu",0x30C4), KANA1("ヌ","nu",0x30CC),
        KANA1("フ","fu",0x30D5), KANA1("ム","mu",0x30E0), KANA1("ユ","yu",0x30E6), KANA1("ル","ru",0x30EB), KANA1("ン","n",0x30F3)
    },
    {
        KANA1("エ","e",0x30A8), KANA1("ケ","ke",0x30B1), KANA1("セ","se",0x30BB), KANA1("テ","te",0x30C6), KANA1("ネ","ne",0x30CD),
        KANA1("ヘ","he",0x30D8), KANA1("メ","me",0x30E1), {" "," ",{0,0},0}, KANA1("レ","re",0x30EC), {" "," ",{0,0},0}
    },
    {
        KANA1("オ","o",0x30AA), KANA1("コ","ko",0x30B3), KANA1("ソ","so",0x30BD), KANA1("ト","to",0x30C8), KANA1("ノ","no",0x30CE),
        KANA1("ホ","ho",0x30DB), KANA1("モ","mo",0x30E2), KANA1("ヨ","yo",0x30E8), KANA1("ロ","ro",0x30ED), KANA1("ヲ","(w)o",0x30F2)
    }
};

const KanaEntry katakana_dakuten[5][5] = {
    {
        KANA1("ガ","ga",0x30AC), KANA1("ザ","za",0x30B6), KANA1("ダ","da",0x30C0), KANA1("バ","ba",0x30D0), KANA1("パ","pa",0x30D1)
    },
    {
        KANA1("ギ","gi",0x30AE), KANA1("ジ","ji",0x30B8), KANA1("ヂ","di",0x30C2), KANA1("ビ","bi",0x30D3), KANA1("ピ","pi",0x30D4)
    },
    {
        KANA1("グ","gu",0x30B0), KANA1("ズ","zu",0x30BA), KANA1("ヅ","du",0x30C5), KANA1("ブ","bu",0x30D6), KANA1("プ","pu",0x30D7)
    },
    {
        KANA1("ゲ","ge",0x30B2), KANA1("ゼ","ze",0x30BC), KANA1("デ","de",0x30C7), KANA1("ベ","be",0x30D9), KANA1("ペ","pe",0x30DA)
    },
    {
        KANA1("ゴ","go",0x30B4), KANA1("ゾ","zo",0x30BE), KANA1("ド","do",0x30C9), KANA1("ボ","bo",0x30DC), KANA1("ポ","po",0x30DD)
    }
};

const KanaEntry katakana_comb[6][6] = {
    {
        KANA2("キャ","kya",0x30AD,0x30E3), KANA2("キュ","kyu",0x30AD,0x30E5), KANA2("キョ","kyo",0x30AD,0x30E7),
        KANA2("ギャ","gya",0x30AE,0x30E3), KANA2("ギュ","gyu",0x30AE,0x30E5), KANA2("ギョ","gyo",0x30AE,0x30E7)
    },
    {
        KANA2("シャ","sha",0x30B7,0x30E3), KANA2("シュ","shu",0x30B7,0x30E5), KANA2("ショ","sho",0x30B7,0x30E7),
        KANA2("ジャ","ja",0x30B8,0x30E3),  KANA2("ジュ","ju",0x30B8,0x30E5),  KANA2("ジョ","jo",0x30B8,0x30E7)
    },
    {
        KANA2("チャ","cha",0x30C1,0x30E3), KANA2("チュ","chu",0x30C1,0x30E5), KANA2("チョ","cho",0x30C1,0x30E7),
        KANA2("リャ","rya",0x30EA,0x30E3), KANA2("リュ","ryu",0x30EA,0x30E5), KANA2("リョ","ryo",0x30EA,0x30E7)
    },
    {
        KANA2("ニャ","nya",0x30CB,0x30E3), KANA2("ニュ","nyu",0x30CB,0x30E5), KANA2("ニョ","nyo",0x30CB,0x30E7),
        KANA2("ミャ","mya",0x30DF,0x30E3), KANA2("ミュ","myu",0x30DF,0x30E5), KANA2("ミョ","myo",0x30DF,0x30E7)
    },
    {
        KANA2("ヒャ","hya",0x30D2,0x30E3), KANA2("ヒュ","hyu",0x30D2,0x30E5), KANA2("ヒョ","hyo",0x30D2,0x30E7),
        KANA2("ビャ","bya",0x30D3,0x30E3), KANA2("ビュ","byu",0x30D3,0x30E5), KANA2("ビョ","byo",0x30D3,0x30E7)
    },
    {
        KANA2("ピャ","pya",0x30D4,0x30E3), KANA2("ピュ","pyu",0x30D4,0x30E5), KANA2("ピョ","pyo",0x30D4,0x30E7),
        KANA2("※ャ","Comparación ya",0x203B,0x30E3), KANA2("※ュ","Comparación yu",0x203B,0x30E5), KANA2("※ョ","Comparación-yo",0x203B,0x30E7)

    }
};

#define X_INICIAL  20
#define Y_INICIAL  50
#define ANCHO_CELDA 36
#define ALTO_CELDA  34


void init_ideograma(unsigned int codepoint, bool animar) {
    if (!kanji_load(codepoint, &k)) return;
    if (animar) kanji_anim_init(&anim);
}




void mostrar_ideograma(bool animar, float x, float y, float size) {
    if (animar)
        animate_kanji_at(&k, &anim, x, y, size);
    else
        draw_kanji_static_at(&k, x, y, size);
}




void draw_kana_detail(C3D_RenderTarget *bottom, C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, TablaState *estado)
{
   if(estado->categoria != 2 && estado->categoria != 5){
    DrawRoundedRect(80, 32, 160, 150, 4, themes[currentTheme].cellIdle);
    DrawRoundedRect(80, 32, 160, 150, 4, 0x00000000);
    DrawRoundedRect(159, 32, 2, 150, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(80, 106, 160, 2, 0, themes[currentTheme].borderCell);

    if (!kanaOculto) {
        if (mostrandoTrazos) {
            int done = animate_kanji_at(&k, &anim, 90.0f, 35.0f, 130.0f);
            if (done) mostrandoTrazos = false;
        } else {
            // carga estática solo cuando NO está animando
            KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];
            kanji_load(entry.codepoints[0], &k);
            draw_kanji_static_at(&k, 90.0f, 35.0f, 130.0f);
        }
    }

   } else {
    DrawRoundedRect(20, 32, 270, 150, 4, themes[currentTheme].cellIdle);
    DrawRoundedRect(20, 32, 270, 150, 4, 0x00000000);
    DrawRoundedRect(99, 32, 2, 150, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(20, 106, 270, 2, 0, themes[currentTheme].borderCell);
    DrawRoundedRect(220, 32, 2, 150, 0, themes[currentTheme].borderCell);

    if (!kanaOculto) {
        if (mostrandoTrazos) {
            int done1 = animate_kanji_at(&k,  &anim,  30.0f, 35.0f, 130.0f);
            int done2 = 0;
            if (done1) {
                done2 = animate_kanji_at(&k2, &anim2, 150.0f, 50.0f, 100.0f);
            }

            if (done1 && done2) mostrandoTrazos = false;
        } else {
            KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];
            kanji_load(entry.codepoints[0], &k);
            kanji_load(entry.codepoints[1], &k2);
            draw_kanji_static_at(&k,  30.0f, 35.0f, 130.0f);
            draw_kanji_static_at(&k2, 150.0f, 50.0f, 100.0f);
        }
    }
   }

    // --- TÍTULO ROMAJI ---
    C2D_Text romajiText;
    C2D_TextFontParse(&romajiText, font2, g_staticBuf, get_tabla(estado->categoria, estado->fila)[estado->col].romaji);
    C2D_TextOptimize(&romajiText);
    C2D_DrawText(&romajiText, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 24.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(270, 10, 30, 30, 6, themes[currentTheme].cellIdle);
    C2D_Text btnHelp;
    C2D_TextFontParse(&btnHelp, font2, g_staticBuf, "?");
    C2D_TextOptimize(&btnHelp);
    C2D_DrawText(&btnHelp, C2D_AtBaseline | C2D_AlignCenter, 285.0f, 30.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(10, 197, 95, 36, 8, themes[currentTheme].btnAudio);
    C2D_Text btnAudio;
    C2D_TextFontParse(&btnAudio, font2, g_staticBuf, "Audio");
    C2D_TextOptimize(&btnAudio);
    C2D_DrawText(&btnAudio, C2D_AtBaseline | C2D_AlignCenter, 57.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].btnAudioText);

    DrawRoundedRect(110, 197, 95, 36, 8, themes[currentTheme].cellIdle);
    C2D_Text btnOcultar;
    C2D_TextFontParse(&btnOcultar, font2, g_staticBuf, kanaOculto ? "Mostrar" : "Ocultar");
    C2D_TextOptimize(&btnOcultar);
    C2D_DrawText(&btnOcultar, C2D_AtBaseline | C2D_AlignCenter, 157.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    DrawRoundedRect(210, 197, 95, 36, 8, themes[currentTheme].btnAudio);
    C2D_Text btnTrazos;
    C2D_TextFontParse(&btnTrazos, font2, g_staticBuf, "Trazos");
    C2D_TextOptimize(&btnTrazos);
    C2D_DrawText(&btnTrazos, C2D_AtBaseline | C2D_AlignCenter, 257.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    if (showHelp) {
        DrawRoundedRect(20, 40, 280, 140, 10, themes[currentTheme].cellIdle);
        C2D_Text helpText;
        C2D_TextFontParse(&helpText, font2, g_staticBuf,
            "Uso:\n\n"
            "- Kana grande: lectura principal\n"
            "- Boton Audio: escuchar\n"
            "- Trazos: orden de escritura\n"
            "- L/R: cambiar tabla\n"
            "- D-Pad: mover cursor\n"
        );
        C2D_TextOptimize(&helpText);
        C2D_DrawText(&helpText, C2D_AtBaseline, 30, 60, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
    }
}



void mostrar_tabla(C3D_RenderTarget *top, C3D_RenderTarget *bottom, 
                   C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, 
                   TablaState *estado)
{
    //PANTALLA SUPERIOR
    C2D_SceneBegin(top);

    int max_filas = (estado->categoria == 2 || estado->categoria == 5) ? 6 : 5;
    int max_columnas = (tablaState.categoria == 2 || tablaState.categoria == 5) ? 6 :
                            (tablaState.categoria == 1 || tablaState.categoria == 4) ? 5 : 10;

    int ancho_celda = (estado->categoria == 2 || estado->categoria == 5) ? ANCHO_CELDA*1.5 : ANCHO_CELDA;
    for (int i = 0; i < max_filas; i++) {        // filas
        for (int j = 0; j < max_columnas; j++) {   // columnas


            int cx = X_INICIAL + j * ancho_celda;
            int cy = Y_INICIAL + i * ALTO_CELDA;

            // cursor resaltado
            if (i == estado->fila && j == estado->col)
                DrawRoundedRect(cx - 6, cy - ALTO_CELDA + 8, ancho_celda, ALTO_CELDA, 5, themes[currentTheme].cellCursor);

            // color del texto: blanco si cursor, negro si no
            u32 color = (i == estado->fila && j == estado->col) 
            ? themes[currentTheme].bg 
            : themes[currentTheme].kanaText;
            
            C2D_Text kanaText;
            C2D_TextFontParse(&kanaText, font1, g_staticBuf, get_tabla(estado->categoria, i)[j].kana);
            C2D_TextOptimize(&kanaText);
            C2D_DrawText(&kanaText, C2D_AtBaseline, cx, cy, 0.0f, 1.2f, 1.2f, color);
        }
    }
    C2D_SceneBegin(bottom);
    if (tablaState.seleccionado) {
        // mostrar detalle del kana actual
        draw_kana_detail(bottom, g_staticBuf, font1, font2, estado);
    
    } else {
    // PANTALLA INFERIOR
    

    char debug[100];
    snprintf(debug, sizeof(debug), "fila:%d col:%d cat:%d",
             estado->fila, estado->col, estado->categoria);

    C2D_Text debugText;
    C2D_TextFontParse(&debugText, font2, g_staticBuf, debug);
    C2D_TextOptimize(&debugText);
    C2D_DrawText(&debugText, C2D_AtBaseline, 20, 50, 0.0f, 0.8f, 0.8f, themes[currentTheme].kanaText);
    }
}

static inline void handle_tabla_touch(u32 kDown, int tx, int ty, TablaState *estado)
{
    if (!(kDown & KEY_TOUCH)) return;
    KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];
    
    // --- BOTÓN AUDIO ---
    if (isTouchInRect(tx, ty, 10, 197, 95, 36)) {
        tts_engine_speak(entry.kana, NULL);
    }

    // --- BOTÓN OCULTAR ---
    if (isTouchInRect(tx, ty, 110, 197, 95, 36)) {
        kanaOculto = !kanaOculto;
    }

    // --- BOTÓN TRAZOS ---
if (isTouchInRect(tx, ty, 210, 197, 95, 36)) {
    kanaOculto = false;
    mostrandoTrazos = !mostrandoTrazos;
    if (mostrandoTrazos) {
        kanji_load(entry.codepoints[0], &k);
        kanji_anim_init(&anim);
        if (estado->categoria == 2 || estado->categoria == 5) {
            kanji_load(entry.codepoints[1], &k2);
            kanji_anim_init(&anim2);
        }
    }
}

    // --- BOTÓN HELP ---
    if (isTouchInRect(tx, ty, 270, 10, 30, 30)) {
        showHelp = !showHelp;
    }
}



