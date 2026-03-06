#include "loadFonts.h"


const char **get_menu(int noptn)
{
    static const char *m[4];
    switch (noptn)
    {
        case 0:
            m[0] = "(A) Quiz";
            m[1] = "(B) Diccionario";
            m[2] = "(X) Práctica";
            m[3] = "(Y) Config";
            break;
        case 1:
            m[0] = "(A) Evaluación inicial";
            m[1] = "(B) Simulación NOKEN";
            m[2] = "(X) Quiz temático";
            m[3] = "(Y) Estadísticas";
            break;
        case 11:
            m[0] = "(A) Hiragana";
            m[1] = "(B) Katakana";
            m[2] = "(X) Kanji";
            m[3] = "(Y) Vocabulario inicial";
            break;
        case 12:
            m[0] = "(A) Nivel N5 ";
            m[1] = "(B) Nivel N4 ";
            m[2] = "(X) Nivel N3 ";
            m[3] = "(Y) Nivel N2-N1";
            break;
        case 13:
            m[0] = "(A) Gramática";
            m[1] = "(B) Vocabulario";
            m[2] = "(X) Kanji por nivel";
            m[3] = "(Y) Lectura";
            break;
        case 14:
            m[0] = " (A) Resumen ";
            m[1] = " (B) Ptos fuertes ";
            m[2] = " (X) Obj. sugeridos ";
            m[3] = " (Y) Comparativas";
            break;
        case 2:
            m[0] = " (A) Hirag y Katak.";
            m[1] = " (B) Kanji";
            m[2] = " (X) Vocabulario";
            m[3] = " (Y) Frases Útiles";
            break;
        case 21:
            m[0] = " (A) Tabla completa";
            m[1] = " (B) Escritura y Trazos";
            m[2] = " (X) Práctica Auditiva";
            m[3] = " (Y) Asoc. Visuales";
            break;
        case 22:
            m[0] = " (A) Básicos N5-N4 ";
            m[1] = " (B) Intermedio N3 ";
            m[2] = " (X) Avanzados N2-N1 ";
            m[3] = " (Y) Búsqueda";
            break;
        case 23:
            m[0] = " (A) Palabras comunes ";
            m[1] = " (B) Categorías";
            m[2] = " (X) Verbos y adj. ";
            m[3] = " (Y) Expresiones";
            break;
        case 24:
            m[0] = " (A) Diálogos ";
            m[1] = " (B) Frases para viaje ";
            m[2] = " (X) Etiqueta ";
            m[3] = " (Y) Rptas. rápidas";
            break;
        case 3:
            m[0] = " (A) Escritura";
            m[1] = " (B) Lectura";
            m[2] = " (X) Escucha";
            m[3] = " (Y) Roleplay";
            break;
        case 31:
            m[0] = " (A) Práctica Hiragana";
            m[1] = " (B) Práctica Katakana";
            m[2] = " (X) Práctica Kanji";
            m[3] = " (Y) Creación Frases";
            break;
        case 32:
            m[0] = " (A) Hiragana Katakana ";
            m[1] = " (B) Kanji con furigana ";
            m[2] = " (X) Textos Básicos ";
            m[3] = " (Y) Textos Avanzados";
            break;
        case 33:
            m[0] = " (A) Audios Básicos ";
            m[1] = " (B) Audios Intermedios";
            m[2] = " (X) Audios Avanzados";
            m[3] = " (Y) Dictados";
            break;
        case 34:
            m[0] = " (A) Diálogos ";
            m[1] = " (B) Interacciones NPC(?)";
            m[2] = " (X) Escenarios reales ";
            m[3] = " (Y) Evaluacion Resp.";
            break;
        case 4:
            m[0] = " (A) Idioma";
            m[1] = " (B) Preferencias";
            m[2] = " (X) Gestión Datos";
            m[3] = " (Y) Créditos soporte";
            break;
        case 41:
            m[0] = " (A) Esp";
            m[1] = " (B) Ingles ";
            m[2] = " (X) Jpones";
            m[3] = " (Y) Personalizado";
            break;
        case 42:
            m[0] = " (A) Dificultad ";
            m[1] = " (B) Orden aprendizaje ";
            m[2] = " (X) Modo Oscuro-Claro ";
            m[3] = " (Y) Recordatorios";
            break;
        case 43:
            m[0] = " (A) Guardar Progreso ";
            m[1] = " (B) Cargar Progreso";
            m[2] = " (X) Importar-Exportar ";
            m[3] = " (Y) Reiniciar Progreso";
            break;
        case 44:
            m[0] = " (A) Desarrollador";
            m[1] = " (B) Manual del Usuario";
            m[2] = " (X) Reportar errores";
            m[3] = " (Y) Actualizaciones";
            break;
        default:
            return NULL;
    }
    return m;
}

void display_menu(int bg_sub, int opc)
{
    
    const char **menu_options = get_menu(opc);
    if (!menu_options) return;

    int total = 4;

    uint16_t *fb = bgGetGfxPtr(bg_sub);

    // limpiar pantalla
    dmaFillHalfWords(clrLightBlue, fb, SCREEN_WIDTH_DS * SCREEN_HEIGHT_DS * 2);
    draw_menu(bg_sub, handle_tloz, tloz_0Bitmap, menu_options, total);

    while (1)
    {
        swiWaitForVBlank();
        scanKeys();

        uint32_t keys = keysDown();
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
                    draw_button(bgGetGfxPtr(bg_sub),
                                x, y, button_width, button_height, 2,
                                clrBlue, clrGreen, clrBlue,
                                handle_tloz, tloz_0Bitmap, 256, 256,
                                menu_options[i], x + 10, y);

                    swiWaitForVBlank();
                    swiDelay(10000);

                    // aquí manejas la acción
                    switch (i)
                    {
                        case 0: display_menu(bg_sub, 1); break;
                        case 1: /* abrir diccionario */ break;
                        case 2: /* abrir práctica */ break;
                        case 3: /* abrir config */ break;
                    }
                    return;
                }
                y += button_height + spacing;
            }
        }
    }
}
