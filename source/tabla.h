#ifndef TABLA_H
#define TABLA_H

typedef struct {
    int col;
    int fila;
    int categoria;
    bool seleccionado;
} TablaState;

extern TablaState tablaState;
extern const char* hiragana[5][10];

#endif