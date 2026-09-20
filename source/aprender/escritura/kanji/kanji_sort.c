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