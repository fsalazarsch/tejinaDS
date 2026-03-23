#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include <wchar.h>
#include "functions.h"



float* draw_kbdt(float pos_x, float pos_y, u32 color , u32 color2 , u32 colorText, C2D_Text * g_staticText, float span){
	
	float *coords = (float*)malloc(4 * sizeof(float*));


	//DrawRoundedRect(pos_x, pos_y, 30*span, 30, 5, color);
	DrawRoundedRect(pos_x+1, pos_y+1, 27, 27, 5, color2);
	
	//C2D_DrawRectangle(pos_x+ SCREEN_HEIGHT/12, pos_y, 0, SCREEN_WIDTH, SCREEN_HEIGHT/8, color, color, color2, color2);
	//C2D_DrawRectangle(pos_x+ SCREEN_HEIGHT/12+2, pos_y+2, 0, SCREEN_WIDTH-4, SCREEN_HEIGHT/8-4, color2, color2, color, color);
	
	C2D_DrawText(g_staticText,  C2D_AtBaseline | C2D_WithColor , pos_x, pos_y+25, 1.0f, 0.80f, 1.0f, colorText);
	C2D_TextOptimize(g_staticText);
	



	coords[0] = pos_x+1;
	coords[1] = pos_y+1;
	coords[2] = 27+coords[0];
	coords[3] = 27+coords[1];

	return coords;

	//C2D_DrawText(g_staticText, C2D_AtBaseline | C2D_WithColor | C2D_WordWrap, 20+ SCREEN_HEIGHT/12, 15+SCREEN_HEIGHT/12+SCREEN_HEIGHT/24, 1.0f, 1.0f, 1.0f, colorText);
	//C2D_TextOptimize(g_staticText);
}




void render_kbd(C2D_TextBuf g_staticBuf, C2D_Font font){


	C2D_Text teststring;
	//char *dsplay_kbtabs = "かASDFGHJKL";//poner candado y lupa al final
	
	const char *dsplay_kbtabs[5][11] = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Ð"},
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "*"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", "+", "Ñ"},
        {"Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "_"},
        {"\\", "§", " ", "", "",  "^", "=", "}",  "", "@", "{"}
        
    	};
	float **coords_kbd = (float **)malloc(55 * sizeof(float *));
	float span = 1;

	int coordsx[]= { 9, 36, 63, 90, 117, 144, 171, 198, 225, 252, 279 };



	for (int j = 0; j< 5;j++){
		for (int i = 0;i<11;i++){
				//char str[2] = {dsplay_kbtabs[i][j], '\0'};
				//char str[2] = { (char)(j*10+i+240), '\0'};
				//char str[2] = "ç";
				C2D_TextFontParse(&teststring, font, g_staticBuf, dsplay_kbtabs[j][i]);
				
				coords_kbd[i] = draw_kbdt(coordsx[i], 30*(j+1), clrBlack , clrWhite , clrBlack	, &teststring, span);
				}
	}

}