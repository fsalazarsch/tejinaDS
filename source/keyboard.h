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

#define KANJI_COLOR_BACK themes[currentTheme].bg
#define KANJI_COLOR_BORDER themes[currentTheme].accent
#define KANJI_COLOR_KEY themes[currentTheme].kanaText
#define KANJI_COLOR_PRESSED themes[currentTheme].hintText

void kbd_init();
void kbd_render(C2D_TextBuf g_staticBuf);
void kbd_exit();

#endif