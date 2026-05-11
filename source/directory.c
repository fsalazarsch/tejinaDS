#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include "menu.h"
#include "themes.h"
#include "scene.h"
#include "drawing.h"
#include "lessons/lesson.h"

void lesson_render_block(Lesson* lesson, int index);

//config teclas 	"\n"


//static const char sent[] ="\t\t日\n\t\t本\n\t\t語\n\t\t文\n";

C2D_Font font, font2, fontkbd;;
C2D_TextBuf g_staticBuf, g_dynamicBuf;
C2D_Text teststring;

bool flag_display_menu = 0;
MenuResult menu;
char menusel[100] = "main"; // Array de caracteres mutable
char menu_ant[100] = ""; // Array de caracteres mutable
char currentLessonPath[256];

int lastTouchX, lastTouchY = -1;

int opc = 0;

#define SCREEN_WIDTH_TOP 400
#define SCREEN_HEIGHT_TOP 240
#define SCREEN_WIDTH_BOTTOM 320

ThemeID currentTheme = THEME_DAY;
SceneID currentScene = SCENE_MENU;
TablaState tablaState = {0, 0, 0, false};
Lesson lesson;
Keyboard kbd;
C2D_Text input_text;
static int lessonLoaded = 0;
int currentBlock = 0;

static void sceneInit(void)
{
	// Create two text buffers: one for static text, and another one for
	// dynamic text - the latter will be cleared at each frame.
	g_staticBuf  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	g_dynamicBuf = C2D_TextBufNew(4096);
}

static void sceneRender(char *menusel, C3D_RenderTarget *top, C3D_RenderTarget *bottom, Keyboard kbd, touchPosition touch, u32 kDown, u32 kHeld, u32 kUp)
{
	C2D_TextBufClear(g_staticBuf);

	//menu = display_menu(g_staticBuf, menusel, font, fontkbd, font2, top, bottom);
	//flag_display_menu = 1;

	if (currentScene == SCENE_GRAMMAR_001)
	{
	    if (!lessonLoaded)
	    {
	        strcpy(currentLessonPath, "romfs:/lessons/grammar_sov_001.txt");
	        lesson_load(currentLessonPath, &lesson);
	        currentBlock = 0;
	        lessonLoaded = 1;
	    }

	    if (kDown & KEY_RIGHT) currentBlock++;
	    if (kDown & KEY_LEFT)  currentBlock--;

	    if (kDown & KEY_B)
	    {
	        currentScene = SCENE_MENU;
	        lessonLoaded = 0;   /* para recargar si vuelves */
	    }
	}


 	if (currentScene == SCENE_TABLA_HIRAGANA) {
        mostrar_tabla(top, bottom, g_staticBuf, font2, font, &tablaState);
    	}
    else if (currentScene == SCENE_TEST_KANA) {

    	
    	mostrar_tabla_kanji(top, bottom, g_staticBuf, g_dynamicBuf, font2, font, &tablaState, &kbd, &touch, kDown, kHeld, kUp);
		C2D_SceneBegin(bottom);

		/*
		//kayboard
   			kbd_render(g_staticBuf, &kbd);
	        const char *pressed = kbd_update(&kbd, &touch, kDown, kHeld, kUp);
			if (pressed != NULL) {
			    if (strcmp(pressed, "←") == 0) {
			        
		            int len = strlen(input_buffer);
		            if (len > 0) {
		                do { len--; } while (len > 0 && (input_buffer[len] & 0xC0) == 0x80);
		                input_buffer[len] = '\0';
		            }

			    } else if (strcmp(pressed, "↵") == 0) {
			        // enter
			    } else {
			        // agregar pressed al string de input
			        strncat(input_buffer, pressed, sizeof(input_buffer) - strlen(input_buffer) - 1);
			    }
			}


		C2D_TextBufClear(g_dynamicBuf);  // usa el buffer dinámico para esto
    	C2D_Text dyn_input;
    	C2D_TextFontParse(&dyn_input, font2, g_dynamicBuf, 
                      strlen(input_buffer) > 0 ? input_buffer : "...");
    	C2D_TextOptimize(&dyn_input);
    	C2D_DrawText(&dyn_input, C2D_AtBaseline | C2D_WithColor,
                 20.0f, 60.0f, 0.5f,
                 0.7f, 0.7f,
                 themes[currentTheme].kanaText);*/


			//C2D_TextFontParse(&input_text, font, g_staticBuf, input_buffer);
//C2D_TextOptimize(&input_text);
//C2D_DrawText(&input_text, C2D_AtBaseline | C2D_WithColor, 10.0f, 20.0f, 0.5f, 0.7f, 0.7f, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF));


		} 
	else if(currentScene == SCENE_GRAMMAR_001){

    	//lesson_render(&lesson);
	    C2D_SceneBegin(top);
	    lesson_render_block(&lesson, currentBlock);

 		}
	else {
        	menu = display_menu(g_staticBuf, menusel, font, font2, top, bottom);
        	flag_display_menu = 1;
    	}
	
	//C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	//C2D_SceneBegin(top);


	//for (int i = 0; i<4;i++){
	//	printf("\nMenu display%i %s\n", i, result.menu[i]);
	//	}
	//free_menu(menu);
	
	
	

	//draw_opts();
	//C2D_TextParse(&teststring, g_staticBuf, menu[0]);
	//C2D_TextFontParse(&teststring, font, g_staticBuf, "(A) quiz");
	//draw_btn(10 , 25, 0xFFFF7300, clrWhite, clrBlack, &teststring);
	
	// Generate and draw dynamic text
	//char buf[160];
	//C2D_Text dynText;
	//snprintf(buf, sizeof(buf), "Current 語 text size: %f (Use  to change)", size);
	
	//snprintf(buf, sizeof(buf), "Opcion: %li", opc);
	//C2D_TextParse(&dynText, g_dynamicBuf, buf);
	//C2D_TextOptimize(&dynText);
	//C2D_DrawText(&dynText, C2D_AlignCenter, 200.0f, 220.0f, 0.5f, 0.5f, 0.5f);
}

static void sceneExit(void)
{
	// Delete the text buffers
	C2D_TextBufDelete(g_dynamicBuf);
	C2D_TextBufDelete(g_staticBuf);
}

int main()
{
	gfxInitDefault();
	romfsInit();
	cfguInit(); // Allow C2D_FontLoadSystem to work
	csndInit();

	//Result ndspResult = csndInit();

	// Initialize the libs
	
	font = C2D_FontLoad("romfs:/the-legend-of-zelda-a-link-to-the-past-ext.bcfnt");
	font2 = C2D_FontLoad("romfs:/NotoSansJP-Regular.bcfnt");
	fontkbd = C2D_FontLoad("romfs:/AppleJapaneseKeyboard.bcfnt");

	tts_engine_init("romfs:/nitech_jp_atr503_m001.htsvoice");
	
	kbd_init();
	// Initialize the libs


	

	//consoleInit(GFX_TOP, NULL);

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screen
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Initialize the scene


	sceneInit();


	// Main loop
	while (aptMainLoop())
	{



		hidScanInput();


	    u32 kDown = hidKeysDown();
	    u32 kUp = hidKeysUp();  // Detecta las teclas "levantadas"
	    u32 kHeld = hidKeysHeld();  // Detecta si el stylus está presionado

	    touchPosition touch;


	    if (kHeld & KEY_TOUCH) {
	        hidTouchRead(&touch);
	        lastTouchX = touch.px;  // Guarda la última posición válida
	        lastTouchY = touch.py;
	    }

		if (kUp & KEY_TOUCH) {
        //printf("Stylus levantado en posición: %03d, %03d\n", touch.px, touch.py);

			if (flag_display_menu == 1){
				printf("\n\n\n");

				for (int i = 0; i<4;i++){
					if (( touch.px >= menu.matriz[i][0] ) && ( touch.px <= menu.matriz[i][2] ))
						if (( touch.py >= menu.matriz[i][1] ) && ( touch.py <= menu.matriz[i][3] )){
							
							//printf("Boton %i", i+1 );
							strcpy(menusel, menu.menu[i]);
							
							//printf("\n%s", menu.menu[i]);
							//sceneRender(menusel);
						}
							//sceneRender();

							

				}
			}
		}


		if (kDown & KEY_TOUCH) {
        	printf("\x1b[2;0H%03d; %03d", touch.px, touch.py);
    	}
		
		//if (kHeld & KEY_L){
		//	strcpy(menusel, menu_ant);
		//}



		if (currentScene == SCENE_TEST_KANA) {
    		
    		handle_tabla_touch_kanji(kDown, kHeld, kUp, lastTouchX, lastTouchY, &tablaState);
    		

    		if (kDown & KEY_L){ 
    			if (tablaState.categoria == 0)
    				tablaState.categoria =60;
    			else
    				tablaState.categoria = (tablaState.categoria - 1);
		    	drawing_clear();
			}

    		if (kDown & KEY_R){ 
		    	tablaState.categoria = (tablaState.categoria + 1) % 61;
		    	drawing_clear();
			}

			if (kDown & KEY_RIGHT) {
			    tablaState.col = (tablaState.col + 1) % 10;
			    drawing_clear();
			}

			if (kDown & KEY_LEFT) {
			    tablaState.col = (tablaState.col - 1 + 10) % 10;
			    drawing_clear();
			}

		    if (kDown & KEY_UP) {
			    tablaState.fila = (tablaState.fila - 1 + 5) % 5;
			    drawing_clear();
			}

			if (kDown & KEY_DOWN) {
			    tablaState.fila = (tablaState.fila + 1) % 5;
			    drawing_clear();
			}

    		if (kDown & KEY_B)
        		currentScene = SCENE_MENU;
		}

		if (currentScene == SCENE_TABLA_HIRAGANA) {


		    handle_tabla_touch(kDown, kHeld, kUp, lastTouchX, lastTouchY, &tablaState);

			tablaState.seleccionado = true;
			int max_columnas = 0;
			int max_filas = 0;


		    if (kDown & KEY_L){ 
				tablaState.fila = 0;
				tablaState.col = 0;
		    	tablaState.categoria = (tablaState.categoria - 1 + 6) % 6;
		    	drawing_clear();
			}
		    if (kDown & KEY_R){ 
				tablaState.fila = 0;
				tablaState.col = 0;
		    	tablaState.categoria = (tablaState.categoria + 1) % 6;
		    	drawing_clear();
			}
		    if (kDown & KEY_UP) {
		    	max_filas = (tablaState.categoria == 2 || tablaState.categoria == 5) ? 6 : 5;

			    tablaState.fila = (tablaState.fila - 1 + max_filas) % max_filas;
			    if (strcmp(hiragana[tablaState.fila][tablaState.col].kana, " ") == 0)
			        tablaState.fila = (tablaState.fila - 1 + max_filas) % max_filas;
			    drawing_clear();
			}
			if (kDown & KEY_DOWN) {
				max_filas = (tablaState.categoria == 2 || tablaState.categoria == 5) ? 6 : 5;

			    tablaState.fila = (tablaState.fila + 1) % max_filas;
			    if (strcmp(hiragana[tablaState.fila][tablaState.col].kana, " ") == 0)
			        tablaState.fila = (tablaState.fila + 1) % max_filas;
			    drawing_clear();
			}
			if (kDown & KEY_LEFT) {
				max_columnas = (tablaState.categoria == 2 || tablaState.categoria == 5) ? 6 :
               				(tablaState.categoria == 1 || tablaState.categoria == 4) ? 5 : 10;

			    tablaState.col = (tablaState.col - 1 + max_columnas) % max_columnas;
			    if (strcmp(hiragana[tablaState.fila][tablaState.col].kana, " ") == 0)
			        tablaState.col = (tablaState.col - 1 + max_columnas) % max_columnas;
			    drawing_clear();
			}
			if (kDown & KEY_RIGHT) {
				max_columnas = (tablaState.categoria == 2 || tablaState.categoria == 5) ? 6 :
               (tablaState.categoria == 1 || tablaState.categoria == 4) ? 5 : 10;

			    tablaState.col = (tablaState.col + 1) % max_columnas;
			    if (strcmp(hiragana[tablaState.fila][tablaState.col].kana, " ") == 0)
			        tablaState.col = (tablaState.col + 1) % max_columnas;
			    drawing_clear();
			}
			if (kDown & KEY_A){
			    //tablaState.seleccionado = !tablaState.seleccionado;
			    //tts_engine_speak("はな", "HL");  // flor
				//tts_engine_speak("はな", "LH");  // nariz
				//tts_engine_speak("あたらしい", NULL);  // neutro
			}
				
		    if (kDown & KEY_B) {
		        currentScene = SCENE_MENU;
		        strcpy(menusel, "  Ⓐ Hiragana Katakana"); // vuelve al menú anterior
		    }
		}

		// Respond to user input
		//u32 kDown = hidKeysDown();
		//u32 kHeld = hidKeysHeld();
		/*if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if (kHeld & KEY_L)
			opc = KEY_L;
		else if (kHeld & KEY_R)
			opc = KEY_R;
		else if (kHeld & KEY_X)
			opc = KEY_X;
		else if (kHeld & KEY_Y)
			opc = KEY_Y;
		else if (kHeld & KEY_A)
			opc = KEY_A;
		else if (kHeld & KEY_B)
			opc = KEY_B;
		*/
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		
		C2D_TargetClear(top, themes[currentTheme].accent);
        C2D_SceneBegin(top);

        // Dibujar rectángulo con bordes redondeados
		DrawRoundedRect(2, 2, SCREEN_WIDTH_TOP-4, SCREEN_HEIGHT_TOP-4, 10, themes[currentTheme].borderCell);
		DrawRoundedRect(4, 4, SCREEN_WIDTH_TOP-8, SCREEN_HEIGHT_TOP-8, 10, themes[currentTheme].bg);

		//C2D_TextFontParse(&teststring, font2, g_staticBuf, "蘭 心 \n恋 愛");
		//C2D_TextOptimize(&teststring);	
		//C2D_DrawText(&teststring, C2D_AtBaseline, 70, 70, 0.0f, 3.5f, 3.5f, 0x000000FF);


        //C3D_FrameEnd(0);

		//C2D_TargetClear(top, C2D_Color32(0xEE, 0xEE, 0xEE, 0x00));
		//C2D_SceneBegin(top);

		//draw_axis();
		//draw_opts(g_dynamicBuf, "曇", "心", "恋", "愛");





		C2D_TargetClear(bottom, themes[currentTheme].accent); 
		C2D_SceneBegin(bottom);
		DrawRoundedRect(2, 2, SCREEN_WIDTH_BOTTOM-4, SCREEN_HEIGHT_TOP-4, 10, themes[currentTheme].borderCell);
		DrawRoundedRect(4, 4, SCREEN_WIDTH_BOTTOM-8, SCREEN_HEIGHT_TOP-8, 10, themes[currentTheme].bg);
		
		sceneRender(menusel, top, bottom, kbd, touch, kDown, kHeld, kUp);

		//sceneRender(size);
		C3D_FrameEnd(0);

	}

	

	//free_menu();

	kbd_exit();
	// Deinitialize the scene
	sceneExit();

	// Deinitialize the libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	tts_engine_exit();
	csndExit();
	
	return 0;
}