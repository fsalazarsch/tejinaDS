#include "loadFonts.h"
#include "sections.h"
#include "themes.h"


const char **get_menu(int noptn)
{
    static const char *m[4];
    switch (noptn)
    {
        case 0:
            m[0] = "Ⓐ Quiz";
            m[1] = "Ⓑ Diccionario";
            m[2] = "Ⓧ Práctica";
            m[3] = "Ⓨ Config";
            break;
        case 1:
            m[0] = "Ⓐ Evaluación inicial";
            m[1] = "Ⓑ Simulación NOKEN";
            m[2] = "Ⓧ Quiz temático";
            m[3] = "Ⓨ Estadísticas";
            break;
        case 11:
            m[0] = "Ⓐ Hiragana";
            m[1] = "Ⓑ Katakana";
            m[2] = "Ⓧ Kanji";
            m[3] = "Ⓨ Vocabulario inicial";
            break;
        case 12:
            m[0] = "Ⓐ Nivel N5 ";
            m[1] = "Ⓑ Nivel N4 ";
            m[2] = "Ⓧ Nivel N3 ";
            m[3] = "Ⓨ Nivel N2-N1";
            break;
        case 13:
            m[0] = "Ⓐ Gramática";
            m[1] = "Ⓑ Vocabulario";
            m[2] = "Ⓧ Kanji por nivel";
            m[3] = "Ⓨ Lectura";
            break;
        case 14:
            m[0] = "Ⓐ Resumen ";
            m[1] = "Ⓑ Ptos fuertes ";
            m[2] = "Ⓧ Obj. sugeridos ";
            m[3] = "Ⓨ Comparativas";
            break;
        case 2:
            m[0] = "Ⓐ Hirag y Katak.";
            m[1] = "Ⓑ Kanji";
            m[2] = "Ⓧ Vocabulario";
            m[3] = "Ⓨ Frases útiles";
            break;
        case 21:
            m[0] = "Ⓐ Tabla completa";
            m[1] = "Ⓑ Escritura y Trazos";
            m[2] = "Ⓧ Práctica Auditiva";
            m[3] = "Ⓨ Asoc. Visuales";
            break;
        case 22:
            m[0] = "Ⓐ Básicos N5-N4 ";
            m[1] = "Ⓑ Intermedio N3 ";
            m[2] = "Ⓧ Avanzados N2-N1 ";
            m[3] = "Ⓨ Búsqueda";
            break;
        case 23:
            m[0] = "Ⓐ Palabras comunes ";
            m[1] = "Ⓑ Categorías";
            m[2] = "Ⓧ Verbos y adj. ";
            m[3] = "Ⓨ Expresiones";
            break;
        case 24:
            m[0] = "Ⓐ Diálogos";
            m[1] = "Ⓑ Frases para viaje";
            m[2] = "Ⓧ Etiqueta";
            m[3] = "Ⓨ Rptas. rápidas";
            break;
        case 3:
            m[0] = "Ⓐ Escritura";
            m[1] = "Ⓑ Lectura";
            m[2] = "Ⓧ Escucha";
            m[3] = "Ⓨ Roleplay";
            break;
        case 31:
            m[0] = "Ⓐ Práctica Hiragana";
            m[1] = "Ⓑ Práctica Katakana";
            m[2] = "Ⓧ Práctica Kanji";
            m[3] = "Ⓨ Creación Frases";
            break;
        case 32:
            m[0] = "Ⓐ Hiragana Katakana ";
            m[1] = "Ⓑ Kanji con furigana ";
            m[2] = "Ⓧ Textos Básicos ";
            m[3] = "Ⓨ Textos Avanzados";
            break;
        case 33:
            m[0] = "Ⓐ Audios Básicos ";
            m[1] = "Ⓑ Audios Intermedios";
            m[2] = "Ⓧ Audios Avanzados";
            m[3] = "Ⓨ Dictados";
            break;
        case 34:
            m[0] = "Ⓐ Diálogos ";
            m[1] = "Ⓑ Interacciones NPC(?)";
            m[2] = "Ⓧ Escenarios reales ";
            m[3] = "Ⓨ Evaluacion Resp.";
            break;
        case 4:
            m[0] = "Ⓐ Idioma";
            m[1] = "Ⓑ Preferencias";
            m[2] = "Ⓧ Gestión Datos";
            m[3] = "Ⓨ Créditos";
            break;
        case 41:
            m[0] = "Ⓐ Esp";
            m[1] = "Ⓑ Ingles ";
            m[2] = "Ⓧ Japones";
            m[3] = "Ⓨ Personalizado";
            break;
        case 42:
            m[0] = "Ⓐ Dificultad ";
            m[1] = "Ⓑ Orden aprendizaje ";
            m[2] = "Ⓧ Modo Oscuro-Claro ";
            m[3] = "Ⓨ Recordatorios";
            break;
        case 43:
            m[0] = "Ⓐ Guardar Progreso ";
            m[1] = "Ⓑ Cargar Progreso";
            m[2] = "Ⓧ Importar-Exportar ";
            m[3] = "Ⓨ Reiniciar Progreso";
            break;
        case 44:
            m[0] = "Ⓐ Desarrollador";
            m[1] = "Ⓑ Manual del Usuario";
            m[2] = "Ⓧ Reportar errores";
            m[3] = "Ⓨ Actualizaciones";
            break;
        default:
            return NULL;
    }
    return m;
}

int handle_menu_action(int opc, int button_index)
{
    return opc*10+button_index+1;
}   


void display_menu(int bg_sub, int initial_opc, KanaTheme theme)
{
    int opc = initial_opc;

    while (opc >= 0)
    {
        const char **menu_options = get_menu(opc);
        if (!menu_options) break;
        
        int total = 4;
        uint16_t *fb = bgGetGfxPtr(bg_sub);

        // limpiar pantalla
        dmaFillHalfWords(theme.bg, fb, SCREEN_WIDTH_DS * SCREEN_HEIGHT_DS * 2);
        draw_menu(bg_sub, handle_nunito, nunito_0Bitmap, menu_options, total, theme);

        int menu_changed = 0;

        while (!menu_changed)
        {
            swiWaitForVBlank();
            scanKeys();

            uint32_t keys = keysDown();

            if (keys & KEY_START)
            {
                opc = -1; // salir completamente de la app
                break;
            }
            else if (keys & KEY_L)
            {
                if (opc >= 0) // menú hijo
                    opc /= 10; // volver al menú padre
                menu_changed = 1; // refrescar menú
                break;
            }

            if (keys & KEY_TOUCH)
            {
                touchPosition touch;
                touchRead(&touch);

                int margin_top = 10;
                int spacing = 10;
                int button_height = 35;
                int button_width = SCREEN_WIDTH_DS - 20;
                int x = 10;
                int y = margin_top;

                for (int i = 0; i < total; i++)
                {
                    Button btn = { x, y, button_width, button_height };
                    if (button_hit(&btn, &touch))
                    {
                        draw_button(fb, x, y, button_width, button_height, 2,
                                    handle_nunito, nunito_0Bitmap, 256, 256,
                                    menu_options[i], x + 10, y, theme, 1);

                        swiWaitForVBlank();
                        swiDelay(10000);

                        int next_opc = handle_menu_action(opc, i);
                        //consoleSetCursor(NULL, 0, 50);
                        //printf("La opcion es %i.\n\n", opc);

                        switch(next_opc)
                        {
                            case 111:
                            run_quiz_hiragana();
                            
                            break;
                            case 112:
                            run_quiz_katakana();
                            break;

                        default:
                            opc = next_opc;
                            menu_changed = 1;
                        
                        break;
                        }
                    }
                    y += button_height + spacing;
                }
            }
        }
    }
}