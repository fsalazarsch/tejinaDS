#include "tabla.h"
#include "themes.h"
#include "kanji_data.h"


Kanji k;
KanjiAnimState anim;

extern ThemeID currentTheme;


const KanaEntry hiragana[5][10] = {
    {{"あ","a",0x3042}, {"が","ga",0x304C}, {"さ","sa",0x3055}, {"た","ta",0x305F}, {"な","na",0x306A}, {"は","ha",0x306F}, {"ま","ma",0x307E}, {"や","ya",0x3084}, {"ら","ra",0x3089}, {"わ","wa",0x308F}},
    {{"い","i",0x3044}, {"き","ki",0x304D}, {"し","shi",0x3057}, {"ち","chi",0x3061}, {"に","ni",0x306B}, {"ひ","hi",0x3072}, {"み","mi",0x307F}, {" "," ",0}, {"り","ri",0x308A}, {" "," ",0}},
    {{"う","u",0x3046}, {"く","ku",0x304F}, {"す","su",0x3059}, {"つ","tsu",0x3064}, {"ぬ","nu",0x306C}, {"ふ","fu",0x3075}, {"む","mu",0x3080}, {"ゆ","yu",0x3086}, {"る","ru",0x308B}, {"ん","n",0x3093}},
    {{"え","e",0x3048}, {"け","ke",0x3051}, {"せ","se",0x305B}, {"て","te",0x3066}, {"ね","ne",0x306D}, {"へ","he",0x3078}, {"め","me",0x3081}, {" "," ",0}, {"れ","re",0x308C}, {" "," ",0}},
    {{"お","o",0x304A}, {"こ","ko",0x3053}, {"そ","so",0x305D}, {"と","to",0x3068}, {"の","no",0x306E}, {"ほ","ho",0x307B}, {"も","mo",0x3082}, {"よ","yo",0x3088}, {"ろ","ro",0x308D}, {"を","(w)o",0x3092}},
};

/*const char* hiragana[5][10] = {
    {"あ", "が", "さ", "た", "な", "は", "ま", "や", "ら", "わ"},
    {"い", "き", "し", "ち", "に", "ひ", "み", " ",  "り", " "},
    {"う", "く", "す", "つ", "ぬ", "ふ", "む", "ゆ", "る", "ん"},
    {"え", "け", "せ", "て", "ね", "へ", "め", " ",  "れ", " "},
    {"お", "こ", "そ", "と", "の", "ほ", "も", "よ", "ろ", "を"},
};*/


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
    init_ideograma(hiragana[estado->fila][estado->col].codepoint, false);
    mostrar_ideograma(false, 90.0f, 35.0f, 130.0f);


 // --- TÍTULO ROMAJI ---
    // (por ahora usamos el kana como placeholder hasta tener tabla romaji)
    C2D_Text romajiText;
    C2D_TextFontParse(&romajiText, font2, g_staticBuf, hiragana[estado->fila][estado->col].romaji);
    C2D_TextOptimize(&romajiText);
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

    for (int i = 0; i < 5; i++) {        // filas
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
            C2D_TextFontParse(&kanaText, font1, g_staticBuf, hiragana[i][j].kana);
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



