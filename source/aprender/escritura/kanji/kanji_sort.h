#ifndef KANJI_SORT_H
#define KANJI_SORT_H

#define KANJI_TOTAL 3003

#define FILTRO_NINGUNO 0
#define FILTRO_JLPT    1
#define FILTRO_GRADO   2
#define FILTRO_RADICAL 3

void kanji_sort_init(void);
int  kanji_sort_at(int absIndex);
int  kanji_active_size(void);
void kanji_filter_set(int tipo, int valor);
void kanji_filter_get(int *tipo, int *valor);

#endif