#include "tabla.h"
#include "themes.h"
#include "kanji_data.h"


Kanji k;
KanjiAnimState anim;

extern ThemeID currentTheme;


const KanaEntry hiragana[5][10] = {
    {{"あ","a",0x3042}, {"か","ka",0x304B}, {"さ","sa",0x3055}, {"た","ta",0x305F}, {"な","na",0x306A}, {"は","ha",0x306F}, {"ま","ma",0x307E}, {"や","ya",0x3084}, {"ら","ra",0x3089}, {"わ","wa",0x308F}},
    {{"い","i",0x3044}, {"き","ki",0x304D}, {"し","shi",0x3057}, {"ち","chi",0x3061}, {"に","ni",0x306B}, {"ひ","hi",0x3072}, {"み","mi",0x307F}, {" "," ",0}, {"り","ri",0x308A}, {" "," ",0}},
    {{"う","u",0x3046}, {"く","ku",0x304F}, {"す","su",0x3059}, {"つ","tsu",0x3064}, {"ぬ","nu",0x306C}, {"ふ","fu",0x3075}, {"む","mu",0x3080}, {"ゆ","yu",0x3086}, {"る","ru",0x308B}, {"ん","n",0x3093}},
    {{"え","e",0x3048}, {"け","ke",0x3051}, {"せ","se",0x305B}, {"て","te",0x3066}, {"ね","ne",0x306D}, {"へ","he",0x3078}, {"め","me",0x3081}, {" "," ",0}, {"れ","re",0x308C}, {" "," ",0}},
    {{"お","o",0x304A}, {"こ","ko",0x3053}, {"そ","so",0x305D}, {"と","to",0x3068}, {"の","no",0x306E}, {"ほ","ho",0x307B}, {"も","mo",0x3082}, {"よ","yo",0x3088}, {"ろ","ro",0x308D}, {"を","(w)o",0x3092}},
};

const KanaEntry hiragana_dakuten[5][10] = {
    {{"が","ga",0x304C}, {"ざ","za",0x3056}, {"だ","da",0x3060}, {"ば","ba",0x3070}, {"ぱ","pa",0x3071}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ぎ","gi",0x304E}, {"じ","ji",0x3058}, {"ぢ","di",0x3062}, {"び","bi",0x3073}, {"ぴ","pi",0x3074}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ぐ","gu",0x3050}, {"ず","zu",0x305A}, {"づ","du",0x3065}, {"ぶ","bu",0x3076}, {"ぷ","pu",0x3077}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"げ","ge",0x3052}, {"ぜ","ze",0x305C}, {"で","de",0x3067}, {"べ","be",0x3079}, {"ぺ","pe",0x307A}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ご","go",0x3054}, {"ぞ","zo",0x305E}, {"ど","do",0x3069}, {"ぼ","bo",0x307C}, {"ぽ","po",0x307D}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
};

const KanaEntry hiragana_comb[4][10] = {
    {{"きゃ","kya",0}, {"しゃ","sha",0}, {"ちゃ","cha",0}, {"にゃ","nya",0}, {"ひゃ","hya",0}, {"みゃ","mya",0}, {"りゃ","rya",0}, {"ぎゃ","gya",0}, {"じゃ","ja",0},  {"びゃ","bya",0}},
    {{"きゅ","kyu",0}, {"しゅ","shu",0}, {"ちゅ","chu",0}, {"にゅ","nyu",0}, {"ひゅ","hyu",0}, {"みゅ","myu",0}, {"りゅ","ryu",0}, {"ぎゅ","gyu",0}, {"じゅ","ju",0},  {"びゅ","byu",0}},
    {{"きょ","kyo",0}, {"しょ","sho",0}, {"ちょ","cho",0}, {"にょ","nyo",0}, {"ひょ","hyo",0}, {"みょ","myo",0}, {"りょ","ryo",0}, {"ぎょ","gyo",0}, {"じょ","jo",0},  {"びょ","byo",0}},
    {{" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {"ぴゃ","pya",0}, {"ぴゅ","pyu",0}, {"ぴょ","pyo",0}},
};

const KanaEntry katakana[5][10] = {
    {{"ア","a",0x30A2}, {"カ","ka",0x30AB}, {"サ","sa",0x30B5}, {"タ","ta",0x30BF}, {"ナ","na",0x30CA}, {"ハ","ha",0x30CF}, {"マ","ma",0x30DE}, {"ヤ","ya",0x30E4}, {"ラ","ra",0x30E9}, {"ワ","wa",0x30EF}},
    {{"イ","i",0x30A4}, {"キ","ki",0x30AD}, {"シ","shi",0x30B7}, {"チ","chi",0x30C1}, {"ニ","ni",0x30CB}, {"ヒ","hi",0x30D2}, {"ミ","mi",0x30DF}, {" "," ",0},        {"リ","ri",0x30EA}, {" "," ",0}},
    {{"ウ","u",0x30A6}, {"ク","ku",0x30AF}, {"ス","su",0x30B9}, {"ツ","tsu",0x30C4}, {"ヌ","nu",0x30CC}, {"フ","fu",0x30D5}, {"ム","mu",0x30E0}, {"ユ","yu",0x30E6}, {"ル","ru",0x30EB}, {"ン","n",0x30F3}},
    {{"エ","e",0x30A8}, {"ケ","ke",0x30B1}, {"セ","se",0x30BB}, {"テ","te",0x30C6}, {"ネ","ne",0x30CD}, {"ヘ","he",0x30D8}, {"メ","me",0x30E1}, {" "," ",0},        {"レ","re",0x30EC}, {" "," ",0}},
    {{"オ","o",0x30AA}, {"コ","ko",0x30B3}, {"ソ","so",0x30BD}, {"ト","to",0x30C8}, {"ノ","no",0x30CE}, {"ホ","ho",0x30DB}, {"モ","mo",0x30E2}, {"ヨ","yo",0x30E8}, {"ロ","ro",0x30ED}, {"ヲ","(w)o",0x30F2}},
};

const KanaEntry katakana_dakuten[5][10] = {
    {{"ガ","ga",0x30AC}, {"ザ","za",0x30B6}, {"ダ","da",0x30C0}, {"バ","ba",0x30D0}, {"パ","pa",0x30D1}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ギ","gi",0x30AE}, {"ジ","ji",0x30B8}, {"ヂ","di",0x30C2}, {"ビ","bi",0x30D3}, {"ピ","pi",0x30D4}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"グ","gu",0x30B0}, {"ズ","zu",0x30BA}, {"ヅ","du",0x30C5}, {"ブ","bu",0x30D6}, {"プ","pu",0x30D7}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ゲ","ge",0x30B2}, {"ゼ","ze",0x30BC}, {"デ","de",0x30C7}, {"ベ","be",0x30D9}, {"ペ","pe",0x30DA}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
    {{"ゴ","go",0x30B4}, {"ゾ","zo",0x30BE}, {"ド","do",0x30C9}, {"ボ","bo",0x30DC}, {"ポ","po",0x30DD}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}, {" "," ",0}},
};

const KanaEntry katakana_comb[4][10] = {
    {{"キャ","kya",0}, {"シャ","sha",0}, {"チャ","cha",0}, {"ニャ","nya",0}, {"ヒャ","hya",0}, {"ミャ","mya",0}, {"リャ","rya",0}, {"ギャ","gya",0}, {"ジャ","ja",0},  {"ビャ","bya",0}},
    {{"キュ","kyu",0}, {"シュ","shu",0}, {"チュ","chu",0}, {"ニュ","nyu",0}, {"ヒュ","hyu",0}, {"ミュ","myu",0}, {"リュ","ryu",0}, {"ギュ","gyu",0}, {"ジュ","ju",0},  {"ビュ","byu",0}},
    {{"キョ","kyo",0}, {"ショ","sho",0}, {"チョ","cho",0}, {"ニョ","nyo",0}, {"ヒョ","hyo",0}, {"ミョ","myo",0}, {"リョ","ryo",0}, {"ギョ","gyo",0}, {"ジョ","jo",0},  {"ビョ","byo",0}},
    {{" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {" "," ",0},      {"ピャ","pya",0}, {"ピュ","pyu",0}, {"ピョ","pyo",0}},
};


#define X_INICIAL  20
#define Y_INICIAL  70
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

   
    // --- FONDO CUADRÍCULA ---
    DrawRoundedRect(80, 32, 160, 150, 4, themes[currentTheme].cellIdle);


    // --- BORDE CUADRÍCULA ---
    // borde exterior
    DrawRoundedRect(80, 32, 160, 150, 4, 0x00000000); // transparente, solo borde
    // línea vertical
    // línea horizontal
    // (estas las dibujamos con rectángulos delgados)
    DrawRoundedRect(159, 32, 2, 150, 0, themes[currentTheme].borderCell);  // vertical
    DrawRoundedRect(80, 106, 160, 2, 0, themes[currentTheme].borderCell);  // horizontal

    // --- KANA GRANDE TENUE ---
    //C2D_Text kanaGuia;
    //C2D_TextFontParse(&kanaGuia, font2, g_staticBuf, hiragana[estado->fila][estado->col]);
    //C2D_TextOptimize(&kanaGuia);
    //C2D_DrawText(&kanaGuia, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 178.0f, 0.5f, 7.5f, 7.5f, 0x18000000);
    init_ideograma(get_tabla(estado->categoria, estado->fila)[estado->col].codepoint, false);
    mostrar_ideograma(false, 90.0f, 35.0f, 130.0f);


 // --- TÍTULO ROMAJI ---
    // (por ahora usamos el kana como placeholder hasta tener tabla romaji)
    C2D_Text romajiText;
    C2D_TextFontParse(&romajiText, font2, g_staticBuf, get_tabla(estado->categoria, estado->fila)[estado->col].romaji);    C2D_TextOptimize(&romajiText);
    C2D_DrawText(&romajiText, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 24.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);


    // --- BOTÓN AUDIO ---
    DrawRoundedRect(20, 197, 125, 36, 8, themes[currentTheme].btnAudio);
    C2D_Text btnAudio;
    C2D_TextFontParse(&btnAudio, font2, g_staticBuf, "Audio");
    C2D_TextOptimize(&btnAudio);
    C2D_DrawText(&btnAudio, C2D_AtBaseline | C2D_AlignCenter, 82.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].btnAudioText);

    // --- BOTÓN OCULTAR ---
    DrawRoundedRect(175, 197, 125, 36, 8, themes[currentTheme].cellIdle);
    C2D_Text btnOcultar;
    C2D_TextFontParse(&btnOcultar, font2, g_staticBuf, "Ocultar");
    C2D_TextOptimize(&btnOcultar);
    C2D_DrawText(&btnOcultar, C2D_AtBaseline | C2D_AlignCenter, 237.0f, 222.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);
}



void mostrar_tabla(C3D_RenderTarget *top, C3D_RenderTarget *bottom, 
                   C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, 
                   TablaState *estado)
{
    //PANTALLA SUPERIOR
    C2D_SceneBegin(top);

    int max_filas = (estado->categoria == 2 || estado->categoria == 5) ? 4 : 5;
    for (int i = 0; i < max_filas; i++) {        // filas
        for (int j = 0; j < 10; j++) {   // columnas

            int cx = X_INICIAL + j * ANCHO_CELDA;
            int cy = Y_INICIAL + i * ALTO_CELDA;

            // cursor resaltado
            if (i == estado->fila && j == estado->col)
                DrawRoundedRect(cx - 6, cy - ALTO_CELDA + 8, ANCHO_CELDA, ALTO_CELDA, 5, themes[currentTheme].cellCursor);

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



