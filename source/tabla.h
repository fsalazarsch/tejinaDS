#ifndef TABLA_H
#define TABLA_H

typedef struct {
    int col;
    int fila;
    int categoria;
    bool seleccionado;
} TablaState;

typedef struct {
    const char* kana;
    const char* romaji;
    unsigned int codepoint;
} KanaEntry;

extern TablaState tablaState;
extern const KanaEntry hiragana[5][10];
extern const KanaEntry hiragana_dakuten[5][10];
extern const KanaEntry hiragana_comb[4][10];
extern const KanaEntry katakana[5][10];
extern const KanaEntry katakana_dakuten[5][10];
extern const KanaEntry katakana_comb[4][10];

static inline const KanaEntry* get_tabla(int categoria, int fila) {
    switch(categoria) {
        case 0: return hiragana[fila];
        case 1: return hiragana_dakuten[fila];
        case 2: return hiragana_comb[fila];
        case 3: return katakana[fila];
        case 4: return katakana_dakuten[fila];
        case 5: return katakana_comb[fila];
        default: return hiragana[fila];
    }
}

#endif