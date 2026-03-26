#include "tabla.h"
#include "themes.h"
#include "kanji_data.h"

Kanji k;
KanjiAnimState anim;


extern ThemeID currentTheme;

const char* hiragana[5][10] = {
    {"あ", "が", "さ", "た", "な", "は", "ま", "や", "ら", "わ"},
    {"い", "き", "し", "ち", "に", "ひ", "み", " ",  "り", " "},
    {"う", "く", "す", "つ", "ぬ", "ふ", "む", "ゆ", "る", "ん"},
    {"え", "け", "せ", "て", "ね", "へ", "め", " ",  "れ", " "},
    {"お", "こ", "そ", "と", "の", "ほ", "も", "よ", "ろ", "を"},
};


#define X_INICIAL  20
#define Y_INICIAL  70
#define ANCHO_CELDA 36
#define ALTO_CELDA  34

void draw_kana_detail(C3D_RenderTarget *bottom, C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, TablaState *estado)
{

    // --- TÍTULO ROMAJI ---
    // (por ahora usamos el kana como placeholder hasta tener tabla romaji)
    C2D_Text romajiText;
    C2D_TextFontParse(&romajiText, font1, g_staticBuf, hiragana[estado->fila][estado->col]);
    C2D_TextOptimize(&romajiText);
    C2D_DrawText(&romajiText, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 24.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    // --- FONDO CUADRÍCULA ---
    DrawRoundedRect(80, 32, 160, 150, 4, themes[currentTheme].cellIdle);

    // --- KANA GRANDE TENUE ---
    C2D_Text kanaGuia;
    C2D_TextFontParse(&kanaGuia, font2, g_staticBuf, hiragana[estado->fila][estado->col]);
    C2D_TextOptimize(&kanaGuia);
    C2D_DrawText(&kanaGuia, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 178.0f, 0.5f, 7.5f, 7.5f, 0x18000000);

    // --- BORDE CUADRÍCULA ---
    // borde exterior
    DrawRoundedRect(80, 32, 160, 150, 4, 0x00000000); // transparente, solo borde
    // línea vertical
    // línea horizontal
    // (estas las dibujamos con rectángulos delgados)
    DrawRoundedRect(159, 32, 2, 150, 0, themes[currentTheme].borderCell);  // vertical
    DrawRoundedRect(80, 106, 160, 2, 0, themes[currentTheme].borderCell);  // horizontal

    // --- BOTÓN AUDIO ---
    DrawRoundedRect(20, 197, 125, 36, 8, themes[currentTheme].btnAudio);

    // --- BOTÓN OCULTAR ---
    DrawRoundedRect(175, 197, 125, 36, 8, themes[currentTheme].cellIdle);
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
            C2D_TextFontParse(&kanaText, font1, g_staticBuf, hiragana[i][j]);
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
    C2D_DrawText(&debugText, C2D_AtBaseline, 20, 50, 0.0f, 0.8f, 0.8f, 0xFF000000);
	}
}



void init_ideograma(void) {
    //kanji_load_hiragana_a(&k);
    //kanji_load(0x30B7, &k);  // シ katakana shi
    //kanji_load(0x66C7, &k);  // 曇
    //kanji_load(0x5FC3, &k);  // 心
    //kanji_load(0x604B, &k);  // 恋
    kanji_load(0x611B, &k);  // 愛
    //kanji_load(0x862D, &k);  // 蘭

    kanji_anim_init(&anim);
}

void mostrar_ideograma(void) {


    animate_kanji(&k, &anim);
}

