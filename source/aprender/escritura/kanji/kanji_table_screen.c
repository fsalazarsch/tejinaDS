#include <stdbool.h>
#include "kanji_table_screen.h"
#include "kanji_sort.h"
#include "kanji_radical.h"
#include "lessons\help_screen.h"

int tab_search_selected = 1;

/* botón de ayuda de la pestaña LECTURA (pantalla inferior) */
#define KANJI_LECTURA_HELP_X 270
#define KANJI_LECTURA_HELP_Y  35
#define KANJI_LECTURA_HELP_W  30
#define KANJI_LECTURA_HELP_H  30

static C2D_Font font_input = NULL;
static Kanji k;
static KanjiAnimState kanjAnim;
static bool mostrarKanjiTrazos = false;
static int ind_criterio = 0;
static int ind_scroll = 0;
/* geometría/colores del panel para las pestañas de dibujo */
static void kanji_panel_geom(DrawingViewGeom *g)
{
    g->x = 70; g->y = 80; g->w = 180; g->h = 140;
    g->vdiv1 = 159; g->vdiv2 = -1; g->hdiv = 150;
    g->clear_x = 10; g->clear_y = 35;
    g->help_x = 270; g->help_y = 35;
    g->buttons = NULL;
    g->n_buttons = 0;
}

static DrawingViewColors view_colors(void)
{
    DrawingViewColors c;
    c.cellIdle     = themes[currentTheme].cellIdle;
    c.borderCell   = themes[currentTheme].borderCell;
    c.kanaText     = themes[currentTheme].kanaText;
    c.btnAudio     = themes[currentTheme].btnAudio;
    c.btnAudioText = themes[currentTheme].btnAudioText;
    return c;
}


static const char* ind_jlpt_labels[] = { "Todos", "N5", "N4", "N3", "N2", "N1", "Sin JLPT" };
static const int   ind_jlpt_tipo[]   = { FILTRO_NINGUNO, FILTRO_JLPT, FILTRO_JLPT, FILTRO_JLPT, FILTRO_JLPT, FILTRO_JLPT, FILTRO_JLPT };
static const int   ind_jlpt_valor[]  = { 0, 5, 4, 3, 2, 1, 0 };
static const int   ind_jlpt_n = 7;

static const char* ind_grado_labels[] = { "Todos", "1", "2", "3", "4", "5", "6", "8 (secundaria)", "9 (nombres)" };
static const int   ind_grado_tipo[]   = { FILTRO_NINGUNO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO, FILTRO_GRADO };
static const int   ind_grado_valor[]  = { 0, 1, 2, 3, 4, 5, 6, 8, 9 };
static const int   ind_grado_n = 9;

static int ind_tipos[216];
static int ind_vals[216];

static int indices_fill(void)
{
    if (ind_criterio == 0) {
        for (int i = 0; i < ind_jlpt_n; i++) { ind_tipos[i] = ind_jlpt_tipo[i]; ind_vals[i] = ind_jlpt_valor[i]; }
        return ind_jlpt_n;
    }
    if (ind_criterio == 1) {
        for (int i = 0; i < ind_grado_n; i++) { ind_tipos[i] = ind_grado_tipo[i]; ind_vals[i] = ind_grado_valor[i]; }
        return ind_grado_n;
    }
    ind_tipos[0] = FILTRO_NINGUNO; ind_vals[0] = 0;
    for (int i = 1; i <= 214; i++) { ind_tipos[i] = FILTRO_RADICAL; ind_vals[i] = i; }
    return 215;
}

static const char* ind_label(int i)
{
    if (ind_criterio == 0) return ind_jlpt_labels[i];
    return ind_grado_labels[i];
}


int unicode_to_utf8(unsigned int codepoint, char* out) {
    if (codepoint <= 0x7F) {
        out[0] = (char)codepoint;
        out[1] = '\0';
        return 1;
    } else if (codepoint <= 0x7FF) {
        out[0] = (char)(0xC0 | (codepoint >> 6));
        out[1] = (char)(0x80 | (codepoint & 0x3F));
        out[2] = '\0';
        return 2;
    } else if (codepoint <= 0xFFFF) {
        // Aquí caen todos los kanji comunes (CJK básico)
        out[0] = (char)(0xE0 | (codepoint >> 12));
        out[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[2] = (char)(0x80 | (codepoint & 0x3F));
        out[3] = '\0';
        return 3;
    } else {
        // Plano suplementario (kanji raros, no deberías llegar aquí)
        out[0] = (char)(0xF0 | (codepoint >> 18));
        out[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        out[3] = (char)(0x80 | (codepoint & 0x3F));
        out[4] = '\0';
        return 4;
    }
}









void draw_kanji_detail(C3D_RenderTarget *bottom, C2D_TextBuf g_staticBuf, C2D_Font font1, C2D_Font font2, TablaState *estado)
{
    int wide = (estado->categoria == 2 || estado->categoria == 5);

    DrawingViewGeom g;
    g.x = wide ? 20 : 80;
    g.y = 32;
    g.w = wide ? 270 : 160;
    g.h = 150;
    g.vdiv1 = wide ? 99 : 159;
    g.vdiv2 = wide ? 220 : -1;
    g.hdiv = 106;
    g.clear_x = 10; g.clear_y = 10;
    g.help_x = 270; g.help_y = 10;
    g.buttons = NULL;
    g.n_buttons = 0;

    drawing_view_render(g_staticBuf, font2, &g, view_colors());
}

void mostrar_tabla_kanji(C3D_RenderTarget *top, C3D_RenderTarget *bottom, 
                   C2D_TextBuf g_staticBuf, C2D_TextBuf g_dynamicBuf, C2D_Font font1, C2D_Font font2, 
                   TablaState *estado, Keyboard *kbd, touchPosition *touch, u32 kDown, u32 kHeld, u32 kUp) //user la categoria como paginador
{
    //PANTALLA SUPERIOR
    C2D_SceneBegin(top);

    if (font_input == NULL) font_input = kbd_get_font();

    int max_filas = 5;
    int max_columnas = 10;
    int ancho_celda = ANCHO_CELDA;
    int gsize = kanji_active_size();
    for (int i = 0; i < max_filas; i++) {        // filas
        for (int j = 0; j < max_columnas; j++) {   // columnas

            int idx = i * max_columnas + j;
            int cx = X_INICIAL + j * ancho_celda;
            int cy = Y_INICIAL + i * ALTO_CELDA;

            // cursor resaltado
            if (i == estado->fila && j == estado->col) {
                int gidx_cursor = 50 * estado->categoria + idx;
                if (gidx_cursor < gsize)
                    DrawRoundedRect(cx - 6, cy - ALTO_CELDA + 8, ancho_celda, ALTO_CELDA, 5, themes[currentTheme].cellCursor);
            }

            // celda fuera de la lista activa: no hay carácter que dibujar
            int global_index = 50 * estado->categoria + idx;
            if (global_index >= gsize) continue;

            u32 color = (i == estado->fila && j == estado->col) 
            ? themes[currentTheme].bg 
            : themes[currentTheme].kanaText;

            char kanji_utf8[5];
            unicode_to_utf8(kanji_data[kanji_sort_at(global_index)].unicode, kanji_utf8);

            C2D_Text kanaText;
            C2D_TextFontParse(&kanaText, font1, g_staticBuf, kanji_utf8);
            C2D_TextOptimize(&kanaText);
            C2D_DrawText(&kanaText, C2D_AtBaseline, cx, cy, 0.0f, 1.2f, 1.2f, color);
        }
    }

    // indicador de página actual (parte inferior de la pantalla superior)
    char page_indicator[32];
    int total_pages = (gsize + 50 - 1) / 50;
    snprintf(page_indicator, sizeof(page_indicator), "Pagina %d/%d",
             estado->categoria + 1, total_pages);

    C2D_Text pageText;
    C2D_TextFontParse(&pageText, font2, g_staticBuf, page_indicator);
    C2D_TextOptimize(&pageText);
    C2D_DrawText(&pageText, C2D_AtBaseline | C2D_AlignCenter, 200.0f, 228.0f, 0.5f, 0.5f, 0.5f, themes[currentTheme].kanaText);

    // filtro activo (indicador)
    int ftipo, fvalor;
    kanji_filter_get(&ftipo, &fvalor);
    if (ftipo != FILTRO_NINGUNO) {
        char f_ind[48];
        if (ftipo == FILTRO_JLPT)
            snprintf(f_ind, sizeof(f_ind), "Filtro: %s", fvalor == 0 ? "Sin JLPT" : fvalor == 5 ? "N5" : fvalor == 4 ? "N4" : fvalor == 3 ? "N3" : fvalor == 2 ? "N2" : "N1");
        else if (ftipo == FILTRO_GRADO) {
            if (fvalor == 0)       snprintf(f_ind, sizeof(f_ind), "Filtro: Sin grado");
            else if (fvalor == 8)  snprintf(f_ind, sizeof(f_ind), "Filtro: 8 Secundaria");
            else if (fvalor == 9)  snprintf(f_ind, sizeof(f_ind), "Filtro: 9 Nombres");
            else                   snprintf(f_ind, sizeof(f_ind), "Filtro: Grado %d", fvalor);
        } else
            snprintf(f_ind, sizeof(f_ind), "Filtro: Radical %d", fvalor);

        C2D_Text filtroText;
        C2D_TextFontParse(&filtroText, font2, g_staticBuf, f_ind);
        C2D_TextOptimize(&filtroText);
        C2D_DrawText(&filtroText, C2D_AtBaseline, 10.0f, 213.0f, 0.5f, 0.5f, 0.5f, themes[currentTheme].kanaText);
    }

    // búsqueda activa (indicador)
    if (kanji_search_has()) {
        char s_ind[72];
        kanji_search_get(s_ind, sizeof(s_ind));
        char s_full[96];
        snprintf(s_full, sizeof(s_full), "Busqueda: %s", s_ind);

        C2D_Text busquedaText;
        C2D_TextFontParse(&busquedaText, font2, g_staticBuf, s_full);
        C2D_TextOptimize(&busquedaText);
        C2D_DrawText(&busquedaText, C2D_AtBaseline, 10.0f, 226.0f, 0.5f, 0.5f, 0.5f, themes[currentTheme].kanaText);
    }

    static char input_buffer[256] = {0};
    C2D_SceneBegin(bottom);

    DrawRoundedRect(5, 5, 75, 20, 8, tab_search_selected == 1 ?  themes[currentTheme].btnAudio : themes[currentTheme].cellIdle);
    C2D_Text btnAudio;
    C2D_TextFontParse(&btnAudio, font2, g_staticBuf, tab_search_selected == 1 ? "LECTURA" : "Lectura");
    C2D_TextOptimize(&btnAudio);
    C2D_DrawText(&btnAudio, C2D_AtBaseline | C2D_AlignCenter, 42.0f, 22.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

    DrawRoundedRect(85, 5, 75, 20, 8, tab_search_selected == 2 ?  themes[currentTheme].btnAudio : themes[currentTheme].cellIdle);
    C2D_Text btnOcultar;
    C2D_TextFontParse(&btnOcultar, font2, g_staticBuf, tab_search_selected == 2 ? "INDICES" : "Indices");
    C2D_TextOptimize(&btnOcultar);
    C2D_DrawText(&btnOcultar, C2D_AtBaseline | C2D_AlignCenter, 122.0f, 22.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

    DrawRoundedRect(165, 5, 75, 20, 8, tab_search_selected == 4 ?  themes[currentTheme].btnAudio : themes[currentTheme].cellIdle);
    C2D_Text btnBuscar;
    C2D_TextFontParse(&btnBuscar, font2, g_staticBuf, tab_search_selected == 4 ? "BUSCAR" : "Buscar");
    C2D_TextOptimize(&btnBuscar);
    C2D_DrawText(&btnBuscar, C2D_AtBaseline | C2D_AlignCenter, 202.0f, 22.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

    DrawRoundedRect(245, 5, 70, 20, 8, tab_search_selected == 3 ?  themes[currentTheme].btnAudio : themes[currentTheme].cellIdle);
    C2D_Text btnTrazos;
    C2D_TextFontParse(&btnTrazos, font2, g_staticBuf, tab_search_selected == 3 ? "DIBUJAR" : "Dibujar");
    C2D_TextOptimize(&btnTrazos);
    C2D_DrawText(&btnTrazos, C2D_AtBaseline | C2D_AlignCenter, 280.0f, 22.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

    if(tab_search_selected == 1){

    kbd_render(g_staticBuf, kbd, kbd->kb_mode);
    const char *pressed = kbd_update(kbd, touch, kDown, kHeld, kUp);

    if (pressed != NULL) {
        if (strcmp(pressed, "←") == 0) {
            int len = strlen(input_buffer);
            if (len > 0) {
                // Retrocede de forma segura manejando caracteres multibyte UTF-8
                do { len--; } while (len > 0 && (input_buffer[len] & 0xC0) == 0x80);
                input_buffer[len] = '\0';
            }
        } 
        else if (strcmp(pressed, "↵") == 0) {
            // Filtrar la lista con el texto (onyomi/katakana,
            // kunyomi/hiragana, significado/romaji)
            if (strlen(input_buffer) > 0)
                kanji_search_set(input_buffer);
            else
                kanji_search_clear();

            tablaState.categoria = 0;
            tablaState.fila = 0;
            tablaState.col = 0;
            mostrarKanjiTrazos = false;

            if (kanji_search_has() && kanji_active_size() == 0) {
                char msg[160];
                snprintf(msg, sizeof(msg),
                    "No hay kanji con:\n%s\n\nRevisa:\n- onyomi en katakana\n- kunyomi en hiragana\n- significado en romaji",
                    input_buffer);
                help_toggle(msg);
            }
        } 
        else {
            // Si es un carácter normal (Hiragana, Katakana o Romaji), se añade al buffer
            if (strlen(input_buffer) + strlen(pressed) < sizeof(input_buffer) - 1) {
                strcat(input_buffer, pressed);
            }
        }
    }


        C2D_TextBufClear(g_dynamicBuf);  // usa el buffer dinámico para esto
        C2D_Text dyn_input;
        C2D_TextFontParse(&dyn_input, font_input, g_dynamicBuf, 
                      strlen(input_buffer) > 0 ? input_buffer : "...");
        C2D_TextOptimize(&dyn_input);
        C2D_DrawText(&dyn_input, C2D_AtBaseline | C2D_WithColor,
                 20.0f, 60.0f, 1.0f,
                 0.8f, 0.9f,
                 themes[currentTheme].kanaText);

        DrawRoundedRect(KANJI_LECTURA_HELP_X, KANJI_LECTURA_HELP_Y,
                        KANJI_LECTURA_HELP_W, KANJI_LECTURA_HELP_H, 8,
                        themes[currentTheme].cellIdle);
        C2D_Text btnHelp;
        C2D_TextFontParse(&btnHelp, font2, g_staticBuf, "?");
        C2D_TextOptimize(&btnHelp);
        C2D_DrawText(&btnHelp, C2D_AtBaseline | C2D_AlignCenter,
                     KANJI_LECTURA_HELP_X + KANJI_LECTURA_HELP_W * 0.5f,
                     KANJI_LECTURA_HELP_Y + KANJI_LECTURA_HELP_H * 0.5f + 7.0f,
                     0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);

    }

    // --- PESTAÑA DIBUJAR: panel cuadriculado + stylus ---
    if (tab_search_selected == 3) {

        DrawingViewGeom g;
        kanji_panel_geom(&g);

        DrawingViewButton tb[1];
        tb[0] = (DrawingViewButton){ 115, 38, 90, 24, 6, mostrarKanjiTrazos ? "TRAZOS" : "Trazos", mostrarKanjiTrazos, 0 };
        g.buttons = tb;
        g.n_buttons = 1;

        drawing_view_render_panel(&g, view_colors());

        // kanji seleccionado (para calcar)
        int global_index = 50 * estado->categoria + (estado->fila * 10 + estado->col);
        if (global_index < kanji_active_size()) {
            int real = kanji_sort_at(global_index);
            if (kanji_load(kanji_data[real].unicode, &k)) {
                if (mostrarKanjiTrazos) {
                    int done = animate_kanji_at(&k, &kanjAnim, 80.0f, 85.0f, 145.0f);
                    if (done) mostrarKanjiTrazos = false;
                } else {
                    draw_kanji_static_at(&k, 80.0f, 85.0f, 145.0f);
                }
            }
        }

        drawing_view_render_tail(g_staticBuf, font2, &g, view_colors());
    }

    // --- PESTAÑA BUSCAR: cuadriculado vacío para buscar por dibujo ---
    if (tab_search_selected == 4) {

        DrawingViewGeom g;
        kanji_panel_geom(&g);

        DrawingViewButton bb[1];
        bb[0] = (DrawingViewButton){ 110, 206, 100, 28, 7, "BUSCAR", 1, 1 };
        g.buttons = bb;
        g.n_buttons = 1;

        drawing_view_render(g_staticBuf, font2, &g, view_colors());
    }

    // --- PESTAÑA INDICES: filtros por JLPT / GRADO / RADICAL ---
    if (tab_search_selected == 2) {

        const char* crit_labels[3] = { "JLPT", "GRADO", "RAD" };
        for (int c = 0; c < 3; c++) {
            u32 bg = (ind_criterio == c) ? themes[currentTheme].btnAudio : themes[currentTheme].cellIdle;
            DrawRoundedRect(10 + c * 80, 30, 70, 24, 6, bg);
            C2D_Text cb;
            C2D_TextFontParse(&cb, font2, g_staticBuf, crit_labels[c]);
            C2D_TextOptimize(&cb);
            C2D_DrawText(&cb, C2D_AtBaseline | C2D_AlignCenter, 45.0f + c * 80.0f, 50.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
        }

        int n = indices_fill();
        int visible = 7;
        int start = ind_scroll;
        if (start + visible > n) start = n - visible;
        if (start < 0) start = 0;

        int ftipo, fvalor;
        kanji_filter_get(&ftipo, &fvalor);

        for (int i = 0; i < visible && start + i < n; i++) {
            int y = 58 + i * 22;
            int es_act = (ind_tipos[start + i] == ftipo && ind_vals[start + i] == fvalor);
            u32 bg = es_act ? themes[currentTheme].btnAudio : themes[currentTheme].cellIdle;
            DrawRoundedRect(10, y, 270, 22, 4, bg);

            if (ind_criterio != 2) {
                C2D_Text it;
                C2D_TextFontParse(&it, font2, g_staticBuf, ind_label(start + i));
                C2D_TextOptimize(&it);
                C2D_DrawText(&it, C2D_AtBaseline, 18.0f, y + 15.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
            } else {
                char nb[16];
                snprintf(nb, sizeof(nb), "R %d", ind_vals[start + i]);
                C2D_Text nbText;
                C2D_TextFontParse(&nbText, font2, g_staticBuf, nb);
                C2D_TextOptimize(&nbText);
                C2D_DrawText(&nbText, C2D_AtBaseline, 18.0f, y + 15.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

                if (ind_vals[start + i] >= 1 && ind_vals[start + i] <= 214) {
                    C2D_Text rc;
                    C2D_TextFontParse(&rc, font1, g_staticBuf, kanji_radical_char[ind_vals[start + i]]);
                    C2D_TextOptimize(&rc);
                    C2D_DrawText(&rc, C2D_AtBaseline | C2D_AlignRight, 272.0f, y + 15.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);
                }
            }
        }

        // flechas de desplazamiento (solo lista radical)
        if (n > visible) {
            DrawRoundedRect(284, 58, 26, 22, 4, themes[currentTheme].cellIdle);
            C2D_Text upArrow;
            C2D_TextFontParse(&upArrow, font2, g_staticBuf, "^");
            C2D_TextOptimize(&upArrow);
            C2D_DrawText(&upArrow, C2D_AtBaseline | C2D_AlignCenter, 297.0f, 76.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);

            DrawRoundedRect(284, 186, 26, 22, 4, themes[currentTheme].cellIdle);
            C2D_Text downArrow;
            C2D_TextFontParse(&downArrow, font2, g_staticBuf, "v");
            C2D_TextOptimize(&downArrow);
            C2D_DrawText(&downArrow, C2D_AtBaseline | C2D_AlignCenter, 297.0f, 204.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
        }

        // contador de kanji que cumplen el filtro
        char countStr[24];
        snprintf(countStr, sizeof(countStr), "%d kanji", kanji_active_size());
        C2D_Text countText;
        C2D_TextFontParse(&countText, font2, g_staticBuf, countStr);
        C2D_TextOptimize(&countText);
        C2D_DrawText(&countText, C2D_AtBaseline | C2D_AlignCenter, 160.0f, 224.0f, 0.5f, 0.6f, 0.6f, themes[currentTheme].kanaText);
    }


    if (tablaState.seleccionado) {
        // mostrar detalle del kana actual
        //draw_kana_detail(bottom, g_staticBuf, font1, font2, estado);
    
    } else {
    // PANTALLA INFERIOR
    /*    
    DrawRoundedRect(10, 10, 30, 30, 6, themes[currentTheme].cellIdle);
    C2D_Text btnHelp;
    C2D_TextFontParse(&btnHelp, font2, g_staticBuf, "<<");
    C2D_TextOptimize(&btnHelp);
    C2D_DrawText(&btnHelp, C2D_AtBaseline | C2D_AlignCenter, 20.0f, 30.0f, 0.5f, 0.8f, 0.8f, themes[currentTheme].kanaText);



    char debug[100];
    snprintf(debug, sizeof(debug), "fila:%d col:%d cat:%d",
             estado->fila, estado->col, estado->categoria);

    C2D_Text debugText;
    C2D_TextFontParse(&debugText, font2, g_staticBuf, debug);
    C2D_TextOptimize(&debugText);
    C2D_DrawText(&debugText, C2D_AtBaseline, 20, 50, 0.0f, 0.8f, 0.8f, themes[currentTheme].kanaText);*/
    }
}

void handle_tabla_touch_kanji(u32 kDown, u32 kHeld, u32 kUp, int tx, int ty, TablaState *estado)
{
    
    //KanaEntry entry = get_tabla(estado->categoria, estado->fila)[estado->col];

    // stylus en las pestañas de dibujo (panel cuadriculado)
    if (tab_search_selected == 3 || tab_search_selected == 4) {
        DrawingViewGeom gum;
        kanji_panel_geom(&gum);
        drawing_view_update(kHeld, kUp, tx, ty, &gum);
    }

    if (!(kDown & KEY_TOUCH)) return;
    
    if (tab_search_selected == 1) {
        if (isTouchInRect(tx, ty, KANJI_LECTURA_HELP_X, KANJI_LECTURA_HELP_Y,
                          KANJI_LECTURA_HELP_W, KANJI_LECTURA_HELP_H)) {
            help_toggle(
                "Busca kanji por su lectura:\n"
                "- Onyomi: escríbelo en katakana\n"
                "- Kunyomi: escríbelo en hiragana\n"
                "- Significado: en romaji\n\n"
                "Teclas かな / カナ / ㋺ cambian de modo\n"
                "↵ busca · ← borra · SELECT salir"
            );
            return;
        }
    }
    
    if (isTouchInRect(tx, ty, 5, 5, 75, 20)) {
         tab_search_selected = 1;


    }

    if (isTouchInRect(tx, ty, 85, 5, 75, 20)) {
         tab_search_selected = 2;
    }

    if (isTouchInRect(tx, ty, 165, 5, 75, 20)) {
         tab_search_selected = 4;
    }

    if (isTouchInRect(tx, ty, 245, 5, 70, 20)) {
         tab_search_selected = 3;
    }

    if (tab_search_selected == 3 || tab_search_selected == 4) {
        DrawingViewGeom g;
        kanji_panel_geom(&g);

        DrawingViewButton kb[1];
        if (tab_search_selected == 3) {
            kb[0] = (DrawingViewButton){ 115, 38, 90, 24, 6, "", mostrarKanjiTrazos, 0 };
            g.buttons = kb;
            g.n_buttons = 1;
        } else {
            kb[0] = (DrawingViewButton){ 110, 206, 100, 28, 7, "", 1, 1 };
            g.buttons = kb;
            g.n_buttons = 1;
        }

        int pressed = drawing_view_touch(tx, ty, &g);
        switch (pressed) {
            case DRAW_VIEW_TOUCH_CLEAR:
                drawing_view_clear();
                break;
            case DRAW_VIEW_TOUCH_HELP:
                help_toggle(
                    tab_search_selected == 3
                    ? "Dibuja el kanji con el stylus\n"
                      "sobre el cuadriculado.\n\n"
                      "La X borra lo que hiciste.\n"
                      "Con D-Pad y L/R cambias\n"
                      "de kanji a practicar."
                    : "Dibuja un kanji de memoria\n"
                      "y pulsa BUSCAR.\n\n"
                      "La X borra lo que hiciste."
                );
                break;
            case 6:
                mostrarKanjiTrazos = !mostrarKanjiTrazos;
                if (mostrarKanjiTrazos) kanji_anim_init(&kanjAnim);
                break;
            case 7:
                /* búsqueda por dibujo (se implementa aparte) */
                break;
            default:
                break;
        }
    }

    if (tab_search_selected == 2) {
        // --- cambiar criterio (JLPT / GRADO / RAD) ---
        for (int c = 0; c < 3; c++) {
            if (isTouchInRect(tx, ty, 10 + c * 80, 30, 70, 24)) {
                ind_criterio = c;
                ind_scroll = 0;
                return;
            }
        }

        int n = indices_fill();
        int visible = 7;
        int start = ind_scroll;
        if (start + visible > n) start = n - visible;
        if (start < 0) start = 0;

        // --- flechas de desplazamiento (lista radical) ---
        if (n > visible) {
            if (isTouchInRect(tx, ty, 284, 58, 26, 22)) {
                ind_scroll -= 7;
                if (ind_scroll < 0) ind_scroll = 0;
                return;
            }
            if (isTouchInRect(tx, ty, 284, 186, 26, 22)) {
                ind_scroll += 7;
                if (ind_scroll > n - visible) ind_scroll = n - visible;
                return;
            }
        }

        // --- seleccionar valor / aplicar filtro ---
        for (int i = 0; i < visible && start + i < n; i++) {
            if (isTouchInRect(tx, ty, 10, 58 + i * 22, 270, 22)) {
                kanji_filter_set(ind_tipos[start + i], ind_vals[start + i]);
                tablaState.categoria = 0;
                drawing_view_clear();
                return;
            }
        }
    }
}

int kanji_handle_input(u32 kDown, u32 kHeld, u32 kUp, int tx, int ty)
{
    handle_tabla_touch_kanji(kDown, kHeld, kUp, tx, ty, &tablaState);

    if (kDown & KEY_L){ 
        int paginas = (kanji_active_size() + 50 - 1) / 50;
        if (paginas < 1) paginas = 1;
        if (tablaState.categoria == 0)
            tablaState.categoria = paginas - 1;
        else
            tablaState.categoria = (tablaState.categoria - 1);
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_R){ 
        int paginas = (kanji_active_size() + 50 - 1) / 50;
        if (paginas < 1) paginas = 1;
        tablaState.categoria = (tablaState.categoria + 1) % paginas;
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_RIGHT) {
        tablaState.col = (tablaState.col + 1) % 10;
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_LEFT) {
        tablaState.col = (tablaState.col - 1 + 10) % 10;
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_UP) {
        tablaState.fila = (tablaState.fila - 1 + 5) % 5;
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_DOWN) {
        tablaState.fila = (tablaState.fila + 1) % 5;
        drawing_view_clear();
        mostrarKanjiTrazos = false;
    }

    if (kDown & KEY_SELECT){
        help_hide();
        return 1;
    }

    return 0;
}


