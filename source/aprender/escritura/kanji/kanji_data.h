#ifndef KANJI_DATA_H
#define KANJI_DATA_H
#include <stddef.h>

#define TOTAL_KANJI 3003

typedef struct {
    unsigned int unicode;
    unsigned char grade;
    signed char jlpt;
    unsigned char stroke_count;

    const char* meanings[14];
    unsigned char meanings_len;

    const char* kun[18];
    unsigned char kun_len;

    const char* on[5];
    unsigned char on_len;

    const char* name[25];
    unsigned char name_len;

    const char* variant;
}KanjiEntryTable;
extern const KanjiEntryTable kanji_data[3003];

#endif
