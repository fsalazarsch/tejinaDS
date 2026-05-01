#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include <wchar.h>
#include "functions.h"
#include "themes.h"

extern ThemeID currentTheme;

typedef struct {
    float   key_coords[55][4];  
    char    key_chars[55][8];   
    int     key_count;
    int     kb_mode;            // 0=hiragana, 1=katakana, 2=romaji
    int     kb_shift;           // 0=normal, 1=shift
} Keyboard;

#define KANJI_COLOR_BACK themes[currentTheme].bg
#define KANJI_COLOR_BORDER themes[currentTheme].accent
#define KANJI_COLOR_KEY themes[currentTheme].kanaText
#define KANJI_COLOR_PRESSED themes[currentTheme].hintText

void kbd_init();
void kbd_render(C2D_TextBuf g_staticBuf, Keyboard *kbd);
const char* kbd_update(Keyboard *kbd, touchPosition *touch, u32 kDown, u32 kHeld, u32 kUp);
void kbd_exit();

#endif