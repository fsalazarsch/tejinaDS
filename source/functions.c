#include "functions.h"

void DrawRoundedRect(float x, float y, float w, float h, float radius, u32 color) {
    // Dibujar el cuerpo del rectángulo (área central)
    C2D_DrawRectSolid(x + radius, y, 0, w - 2 * radius, h, color);

    // Dibujar los bordes laterales
    C2D_DrawRectSolid(x, y + radius, 0, radius, h - 2 * radius, color);
    C2D_DrawRectSolid(x + w - radius, y + radius, 0, radius, h - 2 * radius, color);

    // Dibujar los círculos para las esquinas redondeadas
    C2D_DrawCircleSolid(x + radius, y + radius, 0, radius, color);           // Esquina superior izquierda
    C2D_DrawCircleSolid(x + w - radius, y + radius, 0, radius, color);       // Esquina superior derecha
    C2D_DrawCircleSolid(x + radius, y + h - radius, 0, radius, color);       // Esquina inferior izquierda
    C2D_DrawCircleSolid(x + w - radius, y + h - radius, 0, radius, color);   // Esquina inferior derecha
}

void DrawRoundedRectZ(float x, float y, float w, float h, float r,
                             u32 color, float z)
{
    if (r * 2.0f > w) r = w * 0.5f;
    if (r * 2.0f > h) r = h * 0.5f;

    C2D_DrawRectSolid(x + r, y,     z, w - 2.0f * r, h,            color);
    C2D_DrawRectSolid(x,     y + r, z, w,            h - 2.0f * r, color);
    C2D_DrawCircleSolid(x + r,     y + r,     z, r, color);
    C2D_DrawCircleSolid(x + w - r, y + r,     z, r, color);
    C2D_DrawCircleSolid(x + r,     y + h - r, z, r, color);
    C2D_DrawCircleSolid(x + w - r, y + h - r, z, r, color);
}


void draw_axis(){
	u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
	C2D_DrawLine(0, SCREEN_WIDTH/2, clrBlack, SCREEN_HEIGHT, SCREEN_WIDTH/2, clrBlack, 2, 1.0f);		
	C2D_DrawLine(SCREEN_HEIGHT/2, 0, clrBlack, SCREEN_HEIGHT/2, SCREEN_WIDTH, clrBlack, 2, 1.0f);		
}

void draw_opts(C2D_TextBuf g_dynamicBuf, const char* alt1, const char* alt2, const char* alt3, const char* alt4) {
	u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF); 
	C2D_Font font = C2D_FontLoad("romfs:/MPLUS1p-Regular.bcfnt");
	//C2D_Font font = C2D_FontLoad("romfs:/tloz.bcfnt");
	//C2D_Font font = C2D_FontLoad("romfs:/JiyunoTsubasa.bcfnt");

	float size = 3.0f;

	C2D_TextFontParse(&g_staticText[0], font, g_dynamicBuf, alt1);
	C2D_TextOptimize(&g_staticText[0]);	
	C2D_DrawText(&g_staticText[0], C2D_AtBaseline, SCREEN_WIDTH/6, 7*SCREEN_HEIGHT/24, 0.0f, size, size, clrBlack);
	

	C2D_TextFontParse(&g_staticText[1], font, g_dynamicBuf, alt2);
	C2D_TextOptimize(&g_staticText[1]);
	C2D_DrawText(&g_staticText[1], C2D_AtBaseline, 5*SCREEN_WIDTH/6, 7*SCREEN_HEIGHT/24, 1.0f, size ,size, clrBlack);

	C2D_TextFontParse(&g_staticText[2], font, g_dynamicBuf, alt3);
	C2D_TextOptimize(&g_staticText[2]);
	C2D_DrawText(&g_staticText[2], C2D_AtBaseline | C2D_WithColor| C2D_WordWrap, SCREEN_WIDTH/6, 16*SCREEN_HEIGHT/24, 1.0f, size ,size, clrBlack);

	C2D_TextFontParse(&g_staticText[3], font, g_dynamicBuf, alt4);
	C2D_TextOptimize(&g_staticText[3]);
	C2D_DrawText(&g_staticText[3], C2D_AtBaseline | C2D_WithColor| C2D_WordWrap, 5*SCREEN_WIDTH/6, 16*SCREEN_HEIGHT/24, 1.0f, size ,size, clrBlack);	
}


bool isTouchInRect(int touchX, int touchY, int x, int y, int w, int h) {
    return (touchX >= x && touchX <= x + w &&
            touchY >= y && touchY <= y + h);
}