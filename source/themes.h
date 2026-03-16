#ifndef THEMES_H
#define THEMES_H

#include <nds.h>

// ---------------------------------------------------------------------------
// RGB15(r,g,b) — cada canal 0..31
// BIT(15) = bit de transparencia (opaco)
// ---------------------------------------------------------------------------

typedef struct {
    u16 bg;           // fondo general de pantalla
    u16 cellIdle;     // celda sin seleccionar
    u16 cellCursor;   // celda bajo el cursor
    u16 kanaText;     // texto de kana en la tabla
    u16 kanaRef;      // kana grande de referencia (top)
    u16 romajiText;   // texto romaji
    u16 headerText;   // cabeceras de fila/columna
    u16 hintText;     // instrucciones / texto muted
    u16 accent;       // acento principal (linterna / sakura)
    u16 btnAudio;     // botón de audio (fondo)
    u16 btnAudioText; // texto del botón de audio
    u16 watermark;    // kana marca de agua (bottom screen)
    u16 borderCell;   // borde de celda
} KanaTheme;

// ---------------------------------------------------------------------------
// TEMA 0 — NOCHE  (basado en portada oscura, linterna cálida)
// ---------------------------------------------------------------------------
//  #1a0e2e -> R=13  G=7   B=23
//  #2d1b4e -> R=22  G=13  B=39  → R=22>>3=2  ≈ RGB15(5,3,9)
//  Se mapea el rango 0-255 a 0-31 dividiendo por 8

#define N_BG            RGB15( 6,  4, 11) | BIT(15)   // #1a0e2e  fondo tabla
#define N_CELL_IDLE     RGB15(11,  6, 19) | BIT(15)   // #2d1b4e  celdas idle
#define N_CELL_CURSOR   RGB15(22, 11, 34) | BIT(15)   // #5b2d8a  cursor
#define N_KANA_TEXT     RGB15(30, 28, 26) | BIT(15)   // #f0e6d3  texto kana tabla
#define N_KANA_REF      RGB15(30, 28, 26) | BIT(15)   // #f0e6d3  kana ref grande
#define N_ROMAJI        RGB15(29, 20, 16) | BIT(15)   // #e8a040  romaji (acento)
#define N_HEADER        RGB15(18, 14, 24) | BIT(15)   // #4a3860  headers
#define N_HINT          RGB15(18, 14, 24) | BIT(15)   // #4a3860  hints/instrucciones
#define N_ACCENT        RGB15(29, 20,  8) | BIT(15)   // #e8a040  acento linterna
#define N_BTN_AUDIO     RGB15(22, 11, 34) | BIT(15)   // #5b2d8a  fondo botón audio
#define N_BTN_AUDIO_TXT RGB15(30, 28, 26) | BIT(15)   // #f0e6d3  texto botón audio
#define N_WATERMARK     RGB15(11,  6, 19) | BIT(15)   // #2d1b4e  kana marca de agua
#define N_BORDER        RGB15(18, 14, 24) | BIT(15)   // #4a3860  borde celda

// ---------------------------------------------------------------------------
// TEMA 1 — DÍA  (basado en portada diurna, sakura y madera)
// ---------------------------------------------------------------------------
//  #f5ead0 -> R=245 G=234 B=208 -> RGB15(30,29,26)
//  #8b5e2a -> R=139 G=94  B=42  -> RGB15(17,11, 5)

#define D_BG            RGB15(30, 29, 26) | BIT(15)   // #f5ead0  fondo pantalla
#define D_CELL_IDLE     RGB15(29, 28, 23) | BIT(15)   // #efe0bc  fondo botón idle
#define D_CELL_CURSOR   RGB15(29, 26, 21) | BIT(15)   // #e8d5a8  fondo botón seleccionado
#define D_KANA_TEXT     RGB15(10,  6,  1) | BIT(15)   // #2a1a08  texto botón seleccionado
#define D_KANA_REF      RGB15(10,  6,  1) | BIT(15)   // #2a1a08  kana ref grande
#define D_ROMAJI        RGB15(17, 11,  5) | BIT(15)   // #8b5e2a  romaji
#define D_HEADER        RGB15(18, 12,  4) | BIT(15)   // #4a3010  texto botón idle
#define D_HINT          RGB15(24, 21, 14) | BIT(15)   // #c4a870  hints
#define D_ACCENT        RGB15(17, 11,  5) | BIT(15)   // #8b5e2a  borde botón seleccionado
#define D_BTN_AUDIO     RGB15(13, 19, 12) | BIT(15)   // #6a9c60  fondo botón audio
#define D_BTN_AUDIO_TXT RGB15(30, 29, 26) | BIT(15)   // #f5ead0  texto botón audio
#define D_WATERMARK     RGB15(29, 26, 21) | BIT(15)   // #e8d5a8  kana marca de agua
#define D_BORDER        RGB15(24, 21, 14) | BIT(15)   // #c4a870  borde botón idle

// ---------------------------------------------------------------------------
// Array de temas — acceder con themes[THEME_NIGHT] o themes[THEME_DAY]
// ---------------------------------------------------------------------------

typedef enum {
    THEME_NIGHT = 0,
    THEME_DAY   = 1,
    THEME_COUNT = 2
} ThemeID;

static const KanaTheme themes[THEME_COUNT] = {
    // --- THEME_NIGHT ---
    {
        .bg           = N_BG,
        .cellIdle     = N_CELL_IDLE,
        .cellCursor   = N_CELL_CURSOR,
        .kanaText     = N_KANA_TEXT,
        .kanaRef      = N_KANA_REF,
        .romajiText   = N_ROMAJI,
        .headerText   = N_HEADER,
        .hintText     = N_HINT,
        .accent       = N_ACCENT,
        .btnAudio     = N_BTN_AUDIO,
        .btnAudioText = N_BTN_AUDIO_TXT,
        .watermark    = N_WATERMARK,
        .borderCell   = N_BORDER,
    },
    // --- THEME_DAY ---
    {
        .bg           = D_BG,
        .cellIdle     = D_CELL_IDLE,
        .cellCursor   = D_CELL_CURSOR,
        .kanaText     = D_KANA_TEXT,
        .kanaRef      = D_KANA_REF,
        .romajiText   = D_ROMAJI,
        .headerText   = D_HEADER,
        .hintText     = D_HINT,
        .accent       = D_ACCENT,
        .btnAudio     = D_BTN_AUDIO,
        .btnAudioText = D_BTN_AUDIO_TXT,
        .watermark    = D_WATERMARK,
        .borderCell   = D_BORDER,
    },
};

// ---------------------------------------------------------------------------
// Uso:
//   #include "themes.h"
//   ThemeID currentTheme = THEME_NIGHT;
//   u16 color = themes[currentTheme].cellCursor;
// ---------------------------------------------------------------------------

#endif // THEMES_H