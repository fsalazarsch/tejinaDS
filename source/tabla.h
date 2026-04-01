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

#endif