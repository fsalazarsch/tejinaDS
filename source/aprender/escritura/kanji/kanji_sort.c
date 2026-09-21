/*
 * =========================================================
 *   kanji_sort.c  —  ordenación y filtrado de la pantalla
 *                    superior
 *
 *   Construye un orden global (3003) sobre kanji_data.
 *   Por defecto se ordena por JLPT: N5 -> N4 -> N3 -> N2
 *   -> N1 -> sin JLPT al final, con desempate por Unicode.
 *
 *   Sobre ese orden se aplica un filtro opcional
 *   (FILTRO_JLPT / FILTRO_GRADO / FILTRO_RADICAL) y la
 *   pantalla superior navega sobre la lista resultante.
 * =========================================================
 */

#include <stdlib.h>
#include <string.h>

#include "kanji_sort.h"
#include "kanji_data.h"
#include "kanji_radical.h"

static int sort_order[KANJI_TOTAL];
static int sort_ready = 0;

static int active_list[KANJI_TOTAL];
static int active_size = 0;
static int active_ready = 0;

static int filter_tipo = FILTRO_NINGUNO;
static int filter_valor = 0;

static char search_query[256] = "";

static void fold_to_lower_hira(const char* s, char* out, int cap);
static int  kanji_search_match(const KanjiEntryTable* e, const char* qf);

/* ---------------------------------------------------------
   comparador (modo por defecto: JLPT)
   --------------------------------------------------------- */
static int cmp_jlpt(const void* pa, const void* pb)
{
    const KanjiEntryTable* ea = &kanji_data[*(const int*)pa];
    const KanjiEntryTable* eb = &kanji_data[*(const int*)pb];

    /* jlpt mayor => nivel más fácil (N5=5, N4=4, ... N1=1); -1 va al final */
    int d = (int)eb->jlpt - (int)ea->jlpt;
    if (d != 0) return d;

    /* desempate: unicode ascendente */
    return (ea->unicode > eb->unicode) - (ea->unicode < eb->unicode);
}

void kanji_sort_init(void)
{
    for (int i = 0; i < KANJI_TOTAL; i++)
        sort_order[i] = i;

    qsort(sort_order, KANJI_TOTAL, sizeof(int), cmp_jlpt);

    sort_ready = 1;
}

/* ---------------------------------------------------------
   reconstruye la lista activa según el filtro actual
   --------------------------------------------------------- */
static void rebuild_active(void)
{
    if (!sort_ready) kanji_sort_init();

    char qf[256];
    fold_to_lower_hira(search_query, qf, sizeof(qf));

    int n = 0;
    for (int i = 0; i < KANJI_TOTAL; i++) {
        int dk = sort_order[i];
        const KanjiEntryTable* e = &kanji_data[dk];
        int match;

        if (filter_tipo == FILTRO_JLPT) {
            /* valor: 5..1 (nivel) o 0 = sin JLPT (jlpt == -1) */
            match = (filter_valor == 0) ? (e->jlpt == -1) : (e->jlpt == filter_valor);
        } else if (filter_tipo == FILTRO_GRADO) {
            /* valor: 1..9 o 0 = sin grado (grade == 0) */
            match = (filter_valor == 0) ? (e->grade == 0) : (e->grade == filter_valor);
        } else if (filter_tipo == FILTRO_RADICAL) {
            match = (kanji_radical[dk] == filter_valor);
        } else {
            match = 1;
        }

        if (match && search_query[0] != '\0')
            match = kanji_search_match(e, qf);

        if (match)
            active_list[n++] = dk;
    }

    active_size = n;
    active_ready = 1;
}

void kanji_filter_set(int tipo, int valor)
{
    filter_tipo = tipo;
    filter_valor = valor;
    active_ready = 0;
}

void kanji_filter_get(int *tipo, int *valor)
{
    if (tipo)  *tipo = filter_tipo;
    if (valor) *valor = filter_valor;
}

int kanji_active_size(void)
{
    if (!sort_ready) kanji_sort_init();
    if (!active_ready) rebuild_active();
    return active_size;
}

int kanji_sort_at(int absIndex)
{
    int size = kanji_active_size();
    if (absIndex < 0 || absIndex >= size)
        return -1;

    return active_list[absIndex];
}

// Normaliza a minúsculas + hiragana para que el usuario pueda
// escribir onyomi en katakana, kunyomi en hiragana o significado
// en romaji indistintamente.
static int utf8_decode(const unsigned char* p, unsigned int* cp)
{
    if ((*p & 0x80) == 0) { *cp = *p; return 1; }

    int n = 0;
    unsigned int code = 0;
    if      ((*p & 0xE0) == 0xC0) { n = 2; code = *p & 0x1F; }
    else if ((*p & 0xF0) == 0xE0) { n = 3; code = *p & 0x0F; }
    else if ((*p & 0xF8) == 0xF0) { n = 4; code = *p & 0x07; }
    else { *cp = *p; return 1; }

    for (int i = 1; i < n; i++) {
        if ((p[i] & 0xC0) != 0x80) { *cp = p[0]; return 1; }
        code = (code << 6) | (p[i] & 0x3F);
    }
    *cp = code;
    return n;
}

static void fold_to_lower_hira(const char* s, char* out, int cap)
{
    const unsigned char* p = (const unsigned char*)s;
    int o = 0;
    while (*p && o < cap - 4) {
        unsigned int cp;
        int adv = utf8_decode(p, &cp);

        if (cp >= 0x30A1 && cp <= 0x30F6) cp -= 0x60;   /* katakana -> hiragana */

        if (cp >= 0x3041 && cp <= 0x3096) {             /* hiragana */
            out[o++] = (char)0xE3;
            out[o++] = (char)(0x80 | ((cp >> 6) & 0x3F));
            out[o++] = (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x80) {                          /* ascii lowercase */
            out[o++] = (cp >= 'A' && cp <= 'Z') ? (char)(cp + 32) : (char)cp;
        } else {
            for (int i = 0; i < adv; i++) out[o++] = (char)p[i];
        }
        p += adv;
    }
    out[o] = '\0';
}

static int field_contains(const char* field, const char* qf)
{
    char fold[128];
    fold_to_lower_hira(field, fold, sizeof(fold));
    return strstr(fold, qf) != NULL;
}

// Un kanji coincide si el texto buscado aparece en su onyomi,
// kunyomi o significado (cualquier escritura).
static int kanji_search_match(const KanjiEntryTable* e, const char* qf)
{
    for (int i = 0; i < e->on_len; i++)
        if (field_contains(e->on[i], qf)) return 1;
    for (int i = 0; i < e->kun_len; i++)
        if (field_contains(e->kun[i], qf)) return 1;
    for (int i = 0; i < e->meanings_len; i++)
        if (field_contains(e->meanings[i], qf)) return 1;
    return 0;
}

void kanji_search_set(const char* query)
{
    if (!query) query = "";
    int len = strlen(query);
    if (len >= (int)sizeof(search_query)) len = sizeof(search_query) - 1;
    memcpy(search_query, query, len);
    search_query[len] = '\0';
    active_ready = 0;
}

void kanji_search_clear(void)
{
    search_query[0] = '\0';
    active_ready = 0;
}

int kanji_search_has(void)
{
    return search_query[0] != '\0';
}

void kanji_search_get(char* out, int cap)
{
    if (!out || cap <= 0) return;
    if (cap > (int)sizeof(search_query)) cap = sizeof(search_query);
    memcpy(out, search_query, cap);
    out[cap - 1] = '\0';
}