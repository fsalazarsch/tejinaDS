#ifndef KANJI_DATA_H
#define KANJI_DATA_H

#define TOTAL_KANJI 2770

typedef struct {
    const char *kanji;
    const char *hiragana;
    const char *meaning;
} KanjiWord;

typedef struct {
    unsigned int unicode;
    const char *grade;          /* Ej.: "9k", "1", "6" ("" = sin dato) */
    signed char jlpt;           /* 5=N5 ... 1=N1; 0=sin dato */
    unsigned char stroke_count;
    const char *radical;

    const char *const *meanings;
    unsigned short meanings_len;
    const char *const *kun;
    unsigned short kun_len;
    const char *const *on;
    unsigned short on_len;

    const KanjiWord *common_words;
    unsigned short common_words_len;
    const KanjiWord *uncommon_words;
    unsigned short uncommon_words_len;
} KanjiEntryTable;

extern const KanjiEntryTable kanji_data[TOTAL_KANJI];

#endif
