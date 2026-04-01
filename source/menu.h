#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include "functions.h"
#include "keyboard.h"
#include "themes.h"
#include "menu_functions.h"
#include "tabla.h"
#include "scene.h"

extern ThemeID currentTheme;

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320





typedef struct {
    float **matriz;  // Matriz devuelta por los botones
    char **menu;     // Menú original
} MenuResult;

MenuResult result;

void free_menu(float **matriz) {
    // Liberar cada fila de la matriz
    for (int i = 0; i < 4; i++) {
        free(matriz[i]);

    }

    // Liberar la matriz en sí
    free(matriz);
    //menu = NULL;
}



float* draw_btn(float pos_x, float pos_y, u32 color , u32 color2 , u32 colorText, C2D_Text * g_staticText ){
	
	float *coords = (float*)malloc(4 * sizeof(float*));


	DrawRoundedRect(pos_x+ SCREEN_HEIGHT/12, pos_y, SCREEN_WIDTH, SCREEN_HEIGHT/8, 10, color);
	DrawRoundedRect(pos_x+ SCREEN_HEIGHT/12+2, pos_y+2, SCREEN_WIDTH-4, SCREEN_HEIGHT/8-4, 10, color2);

	//C2D_DrawRectangle(pos_x+ SCREEN_HEIGHT/12, pos_y, 0, SCREEN_WIDTH, SCREEN_HEIGHT/8, color, color, color2, color2);
	//C2D_DrawRectangle(pos_x+ SCREEN_HEIGHT/12+2, pos_y+2, 0, SCREEN_WIDTH-4, SCREEN_HEIGHT/8-4, color2, color2, color, color);
	C2D_DrawText(g_staticText,  C2D_AtBaseline | C2D_WithColor , pos_x+ SCREEN_HEIGHT/12, pos_y+SCREEN_HEIGHT/12+SCREEN_HEIGHT/48, 1.0f, 1.0f, 0.9f, colorText);
	C2D_TextOptimize(g_staticText);
	

	coords[0] = pos_x+ SCREEN_HEIGHT/12+2;
	coords[1] = pos_y+2;
	coords[2] = SCREEN_WIDTH-4+coords[0];
	coords[3] = SCREEN_HEIGHT/8-4+coords[1];

	return coords;

	//C2D_DrawText(g_staticText, C2D_AtBaseline | C2D_WithColor | C2D_WordWrap, 20+ SCREEN_HEIGHT/12, 15+SCREEN_HEIGHT/12+SCREEN_HEIGHT/24, 1.0f, 1.0f, 1.0f, colorText);
	//C2D_TextOptimize(g_staticText);
}



MenuResult build_menu(C2D_TextBuf g_staticBuf, char** menu, u32 *colors, C2D_Font font){ 
//crea un menu de 4 botones para pantalla inferior
	C2D_Text teststring;

    result.matriz = (float **)malloc(4 * sizeof(float *));
	result.menu = (char **)malloc(4 * sizeof(char *));


	//draw_opts();
	//C2D_TextParse(&teststring, g_staticBuf, menu[0]);
	if (menu[0] != NULL){
		C2D_TextFontParse(&teststring, font, g_staticBuf, menu[0]);
		result.matriz[0] = draw_btn(10 , 25, colors[0], themes[currentTheme].bg, themes[currentTheme].kanaText, &teststring);	
		result.menu[0] = menu[0];
	}
	else{
		result.matriz[0] = NULL;
		result.menu[0] = NULL;
		free(menu[0]);
		free(result.matriz[0]);
		free(result.menu[0]);

	}

	if (menu[1] != NULL){
	
		C2D_TextFontParse(&teststring, font, g_staticBuf, menu[1]);
		result.matriz[1] = draw_btn(10 , 15+SCREEN_WIDTH/4, colors[1], themes[currentTheme].bg, themes[currentTheme].kanaText, &teststring);
		result.menu[1] = menu[1];
	}
	else{
		result.matriz[1] = NULL;
		result.menu[1] = NULL;
		free(menu[1]);
		free(result.matriz[1]);
		free(result.menu[1]);
	}

	if (menu[2] != NULL){
		C2D_TextFontParse(&teststring, font, g_staticBuf, menu[2]);
		result.matriz[2] =draw_btn(10 , 5+SCREEN_WIDTH/2, colors[2], themes[currentTheme].bg, themes[currentTheme].kanaText, &teststring);
		result.menu[2] = menu[2];
	}
	else{
		result.matriz[2] = NULL;
		result.menu[2] = NULL;
		free(menu[2]);
		free(result.matriz[2]);
		free(result.menu[2]);

	}
	if (menu[3] != NULL){
		C2D_TextFontParse(&teststring, font, g_staticBuf, menu[3]);
		result.matriz[3] = draw_btn(10 , SCREEN_WIDTH*3/4-5, colors[3], themes[currentTheme].bg, themes[currentTheme].kanaText, &teststring);
		result.menu[3] = menu[3];
	}
	else{
		result.matriz[3] = NULL;
		result.menu[3] = NULL;
		free(menu[3]);
		free(result.matriz[3]);
		free(result.menu[3]);

	}
	//read_file();
	return result;

}


MenuResult display_menu(C2D_TextBuf g_staticBuf, const char* optn, C2D_Font font, C2D_Font font2, C2D_Font font3, C3D_RenderTarget *top, C3D_RenderTarget *bottom) {
    char *menu[4];  // El tamaño es 4 ya que tienes 4 opciones de menú
    u32 colors[4];  // El tamaño es 4 ya que tienes 4 colores

    int noptn = -1;

    if (strcmp(optn, "main") == 0){
    	noptn = 0;
    }

   	if (strcmp(optn, " Ⓐ Quiz") == 0)
   		noptn = 1;
   	if (strcmp(optn, " Ⓐ Evaluación inicial") == 0)
   		noptn = 11;
   	if (strcmp(optn, "  Ⓑ  Simulación NOKEN") == 0)
   		noptn = 12;
   	if (strcmp(optn, "  Ⓧ  Quiz temático") == 0)
   		noptn = 13;
   	if (strcmp(optn, "  Ⓨ  Estadísticas") == 0)
   		noptn = 14;

   	if (strcmp(optn, "  Ⓑ  Diccionario") == 0)
   		noptn = 2;
   	if (strcmp(optn, " Ⓐ Hirag y Katak.") == 0)
   		noptn = 21;
   	if (strcmp(optn, "  Ⓑ  Kanji") == 0)
   		noptn = 22;
   	if (strcmp(optn, "  Ⓧ  Vocabulario") == 0)
   		noptn = 23;
   	if (strcmp(optn, "  Ⓨ  Frases Útiles") == 0)
   		noptn = 24;

   	if (strcmp(optn, "  Ⓧ  Práctica") == 0)
   		noptn = 3;
   	if (strcmp(optn, " Ⓐ Escritura") == 0)
   		noptn = 31;
   	if (strcmp(optn, "  Ⓑ  Lectura") == 0)
   		noptn = 32;
   	if (strcmp(optn, "  Ⓧ  Escucha") == 0)
   		noptn = 33;
   	if (strcmp(optn, "  Ⓨ  Roleplay") == 0)
   		noptn = 34;

   	if (strcmp(optn, "  Ⓨ  Config") == 0)
   		noptn = 4;
   	if (strcmp(optn, " Ⓐ Idioma") == 0)
   		noptn = 41;
   	if (strcmp(optn, "  Ⓑ  Preferencias") == 0)
   		noptn = 42;
   	if (strcmp(optn, "  Ⓧ  Gestión Datos") == 0)
   		noptn = 43;
   	if (strcmp(optn, "  Ⓨ  Créditos soporte") == 0)
   		noptn = 44;

   	if (strcmp(optn, " Ⓐ Tabla completa") == 0)
   		{
   			currentScene = SCENE_TABLA_HIRAGANA;
		    tablaState.col = 0;
		    tablaState.fila = 0;
		    tablaState.categoria = 0;
		    mostrar_tabla(top, bottom, g_staticBuf, font3, font, &tablaState);
   		}

   	if (strcmp(optn, "  Ⓑ  Escritura y Trazos") == 0)
   		{
   			currentScene = SCENE_TEST_KANA;
   			init_ideograma(0x611B, false);  // 愛 estático
			//init_ideograma(0x611B, true);   // 愛 animado
   		}
   	 

   	//strcpy(menu_ant, optn);
	//char buf[160];
	//snprintf(buf, sizeof(buf), "M: %s", menu_ant);
	//C2D_Text teststring;
	//C2D_TextParse(&teststring, g_staticBuf, buf);
	//C2D_TextOptimize(&teststring);	
	//C2D_DrawText(&teststring, C2D_AtBaseline, 20, 20, 0.0f, 1.0f, 1.0f, 0x000000FF);


    switch(noptn){
    	case 0:
	        menu[0] = " Ⓐ Quiz";
	        menu[1] = "  Ⓑ  Diccionario";
	        menu[2] = "  Ⓧ  Práctica";
	        menu[3] = "  Ⓨ  Config";

	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
    		break;

    	case 1:
	        menu[0] = " Ⓐ Evaluación inicial";
	        menu[1] = "  Ⓑ  Simulación NOKEN";
	        menu[2] = "  Ⓧ  Quiz temático";
	        menu[3] = "  Ⓨ  Estadísticas";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
    		break;
    	case 11:
	        menu[0] = " Ⓐ Hiragana";
	        menu[1] = "  Ⓑ  Katakana";
	        menu[2] = "  Ⓧ  Kanji";
	        menu[3] = "  Ⓨ  Vocabulario inicial";
	        
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
	        break;
    	case 12:
	        menu[0] = " Ⓐ Nivel N5 ";
	        menu[1] = "  Ⓑ  Nivel N4 ";
	        menu[2] = "  Ⓧ  Nivel N3 ";
	        menu[3] = "  Ⓨ  Nivel N2-N1";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
	        break;
    	case 13:
	        menu[0] = " Ⓐ Gramática ";
	        menu[1] = "  Ⓑ  Vocabulario ";
	        menu[2] = "  Ⓧ  Kanji por nivel ";
	        menu[3] = "  Ⓨ  Lectura";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
	        break;
    	case 14:
	        menu[0] = " Ⓐ Resumen ";
	        menu[1] = "  Ⓑ  Ptos fuertes ";
	        menu[2] = "  Ⓧ  Obj. sugeridos ";
	        menu[3] = "  Ⓨ  Comparativas";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;
	        break;

    	case 2:
	        menu[0] = " Ⓐ Hirag y Katak.";
	        menu[1] = "  Ⓑ  Kanji";
	        menu[2] = "  Ⓧ  Vocabulario";
	        menu[3] = "  Ⓨ  Frases Útiles";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
    		break;
    	case 21:
	        menu[0] = " Ⓐ Tabla completa";
	        menu[1] = "  Ⓑ  Escritura y Trazos";
	        menu[2] = "  Ⓧ  Práctica Auditiva";
	        menu[3] = "  Ⓨ  Asoc. Visuales";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 22:
	        menu[0] = " Ⓐ Básicos N5-N4 ";
	        menu[1] = "  Ⓑ  Intermedio N3 ";
	        menu[2] = "  Ⓧ  Avanzados N2-N1 ";
	        menu[3] = "  Ⓨ  Búsqueda";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 23:
	        menu[0] = " Ⓐ Palabras comunes ";
	        menu[1] = "  Ⓑ  Categorías";
	        menu[2] = "  Ⓧ  Verbos y adj. ";
	        menu[3] = "  Ⓨ  Expresiones"; 
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 24:
	        menu[0] = " Ⓐ Diálogos ";
	        menu[1] = "  Ⓑ  Frases para viaje ";
	        menu[2] = "  Ⓧ  Etiqueta ";
	        menu[3] = "  Ⓨ  Rptas. rápidas";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;

    	case 3:
	        menu[0] = " Ⓐ Escritura";
	        menu[1] = "  Ⓑ  Lectura";
	        menu[2] = "  Ⓧ  Escucha";
	        menu[3] = "  Ⓨ  Roleplay";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
    		break;
    	case 31:
	        menu[0] = " Ⓐ Práctica Hiragana";
	        menu[1] = "  Ⓑ  Práctica Katakana";
	        menu[2] = "  Ⓧ  Práctica Kanji";
	        menu[3] = "  Ⓨ  Creación Frases";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 32:
	        menu[0] = " Ⓐ Hiragana Katakana ";
	        menu[1] = "  Ⓑ  Kanji con furigana ";
	        menu[2] = "  Ⓧ  Textos Básicos ";
	        menu[3] = "  Ⓨ  Textos Avanzados";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 33:
	        menu[0] = " Ⓐ Audios Básicos ";
	        menu[1] = "  Ⓑ  Audios Intermedios";
	        menu[2] = "  Ⓧ  Audios Avanzados";
	        menu[3] = "  Ⓨ  Dictados"; 
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 34:
	        menu[0] = " Ⓐ Diálogos ";
	        menu[1] = "  Ⓑ  Interacciones NPC(?)";
	        menu[2] = "  Ⓧ  Escenarios reales ";
	        menu[3] = "  Ⓨ  Evaluacion Resp.";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;

    	case 4:
	        menu[0] = " Ⓐ Idioma";
	        menu[1] = "  Ⓑ  Preferencias";
	        menu[2] = "  Ⓧ  Gestión Datos";
	        menu[3] = "  Ⓨ  Créditos soporte";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
    		break;
    	case 41:
	        menu[0] = " Ⓐ Esp";
	        menu[1] = "  Ⓑ  Ingles ";
	        menu[2] = "  Ⓧ  Jpones";
	        menu[3] = "  Ⓨ  Personalizado"; // la ide es cambiar a qui romaji y kana 
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 42:
	        menu[0] = " Ⓐ Dificultad ";
	        menu[1] = "  Ⓑ  Orden aprendizaje ";
	        menu[2] = "  Ⓧ  Modo Oscuro-Claro ";
	        menu[3] = "  Ⓨ  Recordatorios";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 43:
	        menu[0] = " Ⓐ Guardar Progreso ";
	        menu[1] = "  Ⓑ  Cargar Progreso";
	        menu[2] = "  Ⓧ  Importar-Exportar ";
	        menu[3] = "  Ⓨ  Reiniciar Progreso"; 
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;
    	case 44:
	        menu[0] = " Ⓐ Desarrollador";
	        menu[1] = "  Ⓑ  Manual del Usuario";
	        menu[2] = "  Ⓧ  Reportar errores";
	        menu[3] = "  Ⓨ  Actualizaciones";
	        colors[0] = colors[1] = colors[2] = colors[3] = themes[currentTheme].accent;;
	        break;

    	default: //este deberia ser el termino del menu


			//char buf[160] = "Incio de scene render";
			//C2D_SceneBegin(top);
			//C2D_Text teststring;
			//C2D_TextParse(&teststring, g_staticBuf, buf);
			//C2D_TextOptimize(&teststring);	
			//C2D_DrawText(&teststring, C2D_AtBaseline, 20, 40, 0.0f, 1.0f, 1.0f, 0x000000FF);

			C2D_SceneBegin(bottom);

			
    		//free_menu(result.matriz);
    		//DrawRoundedRect(4, 4, 312, 392, 10, 0xFFFFFFFF);
			
	        //menu[2] = " Ⓧ Práctica__";
	        //menu[3] = " Ⓨ Config__";
	        menu[0] = menu[1] = menu[2] = menu[3] = NULL;
	        colors[0] = colors[1] = colors[2] = colors[3] = clrWhite;
	        

	        render_kbd(g_staticBuf, font2);

    		break;
	        //menu[0] = " Ⓐ Quiz";
	        //menu[1] = " Ⓑ Diccionario";
	        //menu[2] = " Ⓧ Práctica";
	        //menu[3] = " Ⓨ Config";

	        // Asignar los colores correspondientes
	        //colors[0] = clrRed;
	        //colors[1] = clrGreen;
	        //colors[2] = clrBlue;
	        //colors[3] = themes[currentTheme].accent;;
    }


    

	result = build_menu(g_staticBuf, menu, colors, font);
    return result;

}

/*

Menú Principal
├── 1) Quiz
│   ├── 1.1) Evaluación inicial
│   │   ├── 1.1.1) Reconocer Hiragana
│   │   ├── 1.1.2) Reconocer Katakana
│   │   ├── 1.1.3) Reconocer Kanji básicos
│   │   └── 1.1.4) Vocabulario inicial
│   ├── 1.2) Simulación NOKEN
│   │   ├── 1.2.1) Nivel N5 (básico)
│   │   ├── 1.2.2) Nivel N4 (intermedio)
│   │   ├── 1.2.3) Nivel N3 (avanzado)
│   │   └── 1.2.4) Nivel N2/N1 (experto)
│   ├── 1.3) Quiz temático
│   │   ├── 1.3.1) Gramática (partículas y estructuras)
│   │   ├── 1.3.2) Vocabulario por tema
│   │   ├── 1.3.3) Kanji por nivel
│   │   └── 1.3.4) Lectura de frases
│   └── 1.4) Progreso y estadísticas
│       ├── 1.4.1) Resumen de evaluaciones
│       ├── 1.4.2) Puntos fuertes y débiles
│       ├── 1.4.3) Objetivos sugeridos
│       └── 1.4.4) Gráficos comparativos
├── 2) Diccionario
│   ├── 2.1) Hiragana y Katakana
│   │   ├── 2.1.1) Tabla completa
│   │   ├── 2.1.2) Escritura y trazos
│   │   ├── 2.1.3) Práctica de sonidos
│   │   └── 2.1.4) Asociaciones visuales
│   ├── 2.2) Kanji
│   │   ├── 2.2.1) Kanji básicos (N5/N4)
│   │   ├── 2.2.2) Kanji intermedios (N3)
│   │   ├── 2.2.3) Kanji avanzados (N2/N1)
│   │   └── 2.2.4) Herramienta de búsqueda por trazos
│   ├── 2.3) Vocabulario
│   │   ├── 2.3.1) Palabras comunes
│   │   ├── 2.3.2) Categorías (números, colores, etc.)
│   │   ├── 2.3.3) Verbos y adjetivos esenciales
│   │   └── 2.3.4) Expresiones idiomáticas
│   └── 2.4) Frases útiles
│       ├── 2.4.1) Diálogos cotidianos
│       ├── 2.4.2) Frases de supervivencia (viajes)
│       ├── 2.4.3) Honoríficos y formalidades
│       └── 2.4.4) Respuestas rápidas
├── 3) Práctica
│   ├── 3.1) Escritura
│   │   ├── 3.1.1) Práctica de Hiragana
│   │   ├── 3.1.2) Práctica de Katakana
│   │   ├── 3.1.3) Práctica de Kanji
│   │   └── 3.1.4) Creación de frases
│   ├── 3.2) Lectura
│   │   ├── 3.2.1) Hiragana y Katakana
│   │   ├── 3.2.2) Kanji con furigana
│   │   ├── 3.2.3) Lectura de textos básicos
│   │   └── 3.2.4) Lectura de textos avanzados
│   ├── 3.3) Escucha
│   │   ├── 3.3.1) Audios básicos
│   │   ├── 3.3.2) Audios intermedios
│   │   ├── 3.3.3) Audios avanzados
│   │   └── 3.3.4) Dictados
│   └── 3.4) Roleplay
│       ├── 3.4.1) Diálogos básicos
│       ├── 3.4.2) Interacción con NPCs
│       ├── 3.4.3) Escenarios reales (compras, viajes)
│       └── 3.4.4) Evaluación de respuestas
└── 4) Configuración
    ├── 4.1) Idioma
    │   ├── 4.1.1) Español
    │   ├── 4.1.2) Inglés
    │   ├── 4.1.3) Japonés
    │   └── 4.1.4) Personalizado
    ├── 4.2) Preferencias
    │   ├── 4.2.1) Dificultad de quizzes
    │   ├── 4.2.2) Orden de aprendizaje
    │   ├── 4.2.3) Modo oscuro/claro
    │   └── 4.2.4) Recordatorios diarios
    ├── 4.3) Gestión de datos
    │   ├── 4.3.1) Guardar progreso
    │   ├── 4.3.2) Cargar progreso
    │   ├── 4.3.3) Exportar/importar datos
    │   └── 4.3.4) Reiniciar progreso
    └── 4.4) Créditos y soporte
        ├── 4.4.1) Equipo desarrollador
        ├── 4.4.2) Manual del usuario
        ├── 4.4.3) Reportar errores
        └── 4.4.4) Actualizaciones

*/


