#ifndef THEMES_H
#define THEMES_H

#include <3ds.h>
#include <citro2d.h>

/* u32 colores en formato 0xAABBGGRR (citro2d) */

typedef struct {
    u32 bg;
    u32 cellIdle;
    u32 cellCursor;
    u32 kanaText;
    u32 kanaRef;
    u32 romajiText;
    u32 headerText;
    u32 hintText;
    u32 accent;
    u32 btnAudio;
    u32 btnAudioText;
    u32 watermark;
    u32 borderCell;
} KanaTheme;

typedef enum {
    THEME_NIGHT = 0,
    THEME_DAY   = 1,
    THEME_COUNT = 2
} ThemeID;

/* Valores hex directos: 0xAABBGGRR */
static const KanaTheme themes[THEME_COUNT] = {
    /* THEME_NIGHT */
    {
        .bg           = 0xFF2e0e1a,
        .cellIdle     = 0xFF4e1b2d,
        .cellCursor   = 0xFF8a2d5b,
        .kanaText     = 0xFFd3e6f0,
        .kanaRef      = 0xFFd3e6f0,
        .romajiText   = 0xFF40a0e8,
        .headerText   = 0xFF603848,
        .hintText     = 0xFF603848,
        .accent       = 0xFF40a0e8,
        .btnAudio     = 0xFF8a2d5b,
        .btnAudioText = 0xFFd3e6f0,
        .watermark    = 0xFF4e1b2d,
        .borderCell   = 0xFF603848,
    },
    /* THEME_DAY */
    {
        .bg           = 0xFFd0eaf5,
        .cellIdle     = 0xFFbce0ef,
        .cellCursor   = 0xFFa8d5e8,
        .kanaText     = 0xFF081a2a,
        .kanaRef      = 0xFF081a2a,
        .romajiText   = 0xFF2a5e8b,
        .headerText   = 0xFF10304a,
        .hintText     = 0xFF70a8c4,
        .accent       = 0xFF2a5e8b,
        .btnAudio     = 0xFF609c6a,
        .btnAudioText = 0xFFd0eaf5,
        .watermark    = 0xFFa8d5e8,
        .borderCell   = 0xFF70a8c4,
    },
};

#endif /* THEMES_H */