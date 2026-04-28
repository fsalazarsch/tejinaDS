
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ===== ENUM =====
enum colors {
	clrBlue = 0xFFFCCA9A,
	clrBlack = 0xFF000000,
	clrWhite = 0xFFFFFFFF,
	clrRed = 0xFF0000FF,
	clrGreen = 0xFF00601C,
	clrBlueclr = 0xFFFF7300
};

// ===== CONSTANTES =====
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// ===== GLOBALS (DECLARACIÓN) =====
extern C2D_Text g_staticText[4];
extern char buf[160];

// ===== STRUCT =====
struct menu_btn {
	char* value;
	u32 color_btn;
	u32 color_txt;
};

// ===== FUNCIONES =====
void DrawRoundedRect(float x, float y, float w, float h, float radius, u32 color);
void draw_axis();
void draw_opts(C2D_TextBuf g_dynamicBuf, const char* alt1, const char* alt2, const char* alt3, const char* alt4);
bool isTouchInRect(int touchX, int touchY, int x, int y, int w, int h);

#endif