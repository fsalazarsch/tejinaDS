/* =========================================================
   FILE: lesson.c
   ========================================================= */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lesson.h"

static void trim_newline(char* str)
{
    int len = strlen(str);

    while(len > 0 &&
         (str[len - 1] == '\n' ||
          str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

static BlockType get_block_type(const char* name)
{
    if(strcmp(name, "dialog")      == 0) return BLOCK_DIALOG;
    if(strcmp(name, "term")        == 0) return BLOCK_TERM;
    if(strcmp(name, "compare")     == 0) return BLOCK_COMPARE;
    if(strcmp(name, "example")     == 0) return BLOCK_EXAMPLE;
    if(strcmp(name, "sequence")    == 0) return BLOCK_SEQUENCE;
    if(strcmp(name, "grammar_box") == 0) return BLOCK_GRAMMAR_BOX;
    if(strcmp(name, "particle")    == 0) return BLOCK_PARTICLE;
    if(strcmp(name, "breakdown")   == 0) return BLOCK_BREAKDOWN;
    if(strcmp(name, "quiz")        == 0) return BLOCK_QUIZ;
    if(strcmp(name, "summary")     == 0) return BLOCK_SUMMARY;
    if(strcmp(name, "sfx")         == 0) return BLOCK_SFX;
    if(strcmp(name, "reward")      == 0) return BLOCK_REWARD;
    if(strcmp(name, "unlock")      == 0) return BLOCK_UNLOCK;

    return BLOCK_NONE;
}

static void set_field(LessonBlock* block,
                      const char*  key,
                      const char*  value)
{
    switch(block->type)
    {
        /* -----------------------------------------------------
           DIALOG
           ----------------------------------------------------- */
        case BLOCK_DIALOG:

            if     (strcmp(key, "portrait") == 0) strcpy(block->DIALOG_PORTRAIT, value);
            else if(strcmp(key, "name")     == 0) strcpy(block->DIALOG_NAME,     value);
            else if(strcmp(key, "text")     == 0) strcpy(block->DIALOG_TEXT,     value);
            break;

        /* -----------------------------------------------------
           TERM
           ----------------------------------------------------- */
        case BLOCK_TERM:

            if     (strcmp(key, "word")    == 0) strcpy(block->TERM_WORD,    value);
            else if(strcmp(key, "meaning") == 0) strcpy(block->TERM_MEANING, value);
            break;

        /* -----------------------------------------------------
           COMPARE
           ----------------------------------------------------- */
        case BLOCK_COMPARE:

            if     (strcmp(key, "left_title")  == 0) strcpy(block->COMPARE_LEFT_TITLE,  value);
            else if(strcmp(key, "left")        == 0) strcpy(block->COMPARE_LEFT,        value);
            else if(strcmp(key, "right_title") == 0) strcpy(block->COMPARE_RIGHT_TITLE, value);
            else if(strcmp(key, "right")       == 0) strcpy(block->COMPARE_RIGHT,       value);
            break;

        /* -----------------------------------------------------
           EXAMPLE
           ----------------------------------------------------- */
        case BLOCK_EXAMPLE:

            if     (strcmp(key, "jp")     == 0) strcpy(block->EXAMPLE_JP,     value);
            else if(strcmp(key, "romaji") == 0) strcpy(block->EXAMPLE_ROMAJI, value);
            else if(strcmp(key, "es")     == 0) strcpy(block->EXAMPLE_ES,     value);
            break;

        /* -----------------------------------------------------
           SEQUENCE
           ----------------------------------------------------- */
        case BLOCK_SEQUENCE:

            if(strcmp(key, "step") == 0) strcpy(block->SEQUENCE_STEP, value);
            break;

        /* -----------------------------------------------------
           GRAMMAR_BOX
           ----------------------------------------------------- */
        case BLOCK_GRAMMAR_BOX:

            if     (strcmp(key, "title")   == 0) strcpy(block->GBOX_TITLE,   value);
            else if(strcmp(key, "content") == 0) strcpy(block->GBOX_CONTENT, value);
            break;

        /* -----------------------------------------------------
           PARTICLE
           ----------------------------------------------------- */
        case BLOCK_PARTICLE:

            if     (strcmp(key, "symbol")   == 0) strcpy(block->PARTICLE_SYMBOL,   value);
            else if(strcmp(key, "name")     == 0) strcpy(block->PARTICLE_NAME,     value);
            else if(strcmp(key, "function") == 0) strcpy(block->PARTICLE_FUNCTION, value);
            break;

        /* -----------------------------------------------------
           BREAKDOWN
           ----------------------------------------------------- */
        case BLOCK_BREAKDOWN:

            if     (strcmp(key, "jp")      == 0) strcpy(block->BREAKDOWN_JP,       value);
            else if(strcmp(key, "line1")   == 0) strcpy(block->BREAKDOWN_LINE1,    value);
            else if(strcmp(key, "line1_es")== 0) strcpy(block->BREAKDOWN_LINE1_ES, value);
            else if(strcmp(key, "line2")   == 0) strcpy(block->BREAKDOWN_LINE2,    value);
            else if(strcmp(key, "line2_es")== 0) strcpy(block->BREAKDOWN_LINE2_ES, value);
            else if(strcmp(key, "line3")   == 0) strcpy(block->BREAKDOWN_LINE3,    value);
            else if(strcmp(key, "line3_es")== 0) strcpy(block->BREAKDOWN_LINE3_ES, value);
            break;

        /* -----------------------------------------------------
           QUIZ
           ----------------------------------------------------- */
        case BLOCK_QUIZ:

            if     (strcmp(key, "question")     == 0) strcpy(block->QUIZ_QUESTION,     value);
            else if(strcmp(key, "a")            == 0) strcpy(block->QUIZ_A,            value);
            else if(strcmp(key, "b")            == 0) strcpy(block->QUIZ_B,            value);
            else if(strcmp(key, "c")            == 0) strcpy(block->QUIZ_C,            value);
            else if(strcmp(key, "correct")      == 0) strcpy(block->QUIZ_CORRECT,      value);
            else if(strcmp(key, "success_text") == 0) strcpy(block->QUIZ_SUCCESS_TEXT, value);
            else if(strcmp(key, "fail_text")    == 0) strcpy(block->QUIZ_FAIL_TEXT,    value);
            break;

        /* -----------------------------------------------------
           SUMMARY
           ----------------------------------------------------- */
        case BLOCK_SUMMARY:

            if     (strcmp(key, "line1") == 0) strcpy(block->SUMMARY_LINE1, value);
            else if(strcmp(key, "line2") == 0) strcpy(block->SUMMARY_LINE2, value);
            else if(strcmp(key, "line3") == 0) strcpy(block->SUMMARY_LINE3, value);
            break;

        /* -----------------------------------------------------
           SFX
           ----------------------------------------------------- */
        case BLOCK_SFX:

            if(strcmp(key, "id") == 0) strcpy(block->SFX_ID, value);
            break;

        /* -----------------------------------------------------
           REWARD
           ----------------------------------------------------- */
        case BLOCK_REWARD:

            if     (strcmp(key, "type")  == 0) strcpy(block->REWARD_TYPE,  value);
            else if(strcmp(key, "value") == 0) strcpy(block->REWARD_VALUE, value);
            break;

        /* -----------------------------------------------------
           UNLOCK
           ----------------------------------------------------- */
        case BLOCK_UNLOCK:

            if(strcmp(key, "lesson") == 0) strcpy(block->UNLOCK_LESSON, value);
            break;

        default:
            break;
    }
}

int lesson_load(const char* filename, Lesson* lesson)
{
    FILE* fp;
    char  line[512];

    LessonBlock* currentBlock = NULL;

    fp = fopen(filename, "r");

    if(!fp)
        return 0;

    memset(lesson, 0, sizeof(Lesson));

    while(fgets(line, sizeof(line), fp))
    {
        trim_newline(line);

        if(strlen(line) == 0)
            continue;

        if(line[0] == '#')
            continue;

        /* =================================================
           METADATA
           ================================================= */

        if(line[0] == '@')
        {
            char* eq = strchr(line, '=');

            if(!eq) continue;

            *eq = '\0';

            char* key   = line + 1;
            char* value = eq + 1;

            if     (strcmp(key, "lesson_id") == 0) strcpy(lesson->lessonId,   value);
            else if(strcmp(key, "title")     == 0) strcpy(lesson->title,      value);
            else if(strcmp(key, "category")  == 0) strcpy(lesson->category,   value);
            else if(strcmp(key, "bg")        == 0) strcpy(lesson->background, value);

            continue;
        }

        /* =================================================
           BLOCK START
           ================================================= */

        if(line[0] == '[')
        {
            char blockName[64];

            sscanf(line, "[%63[^]]", blockName);

            currentBlock = &lesson->blocks[lesson->totalBlocks++];

            memset(currentBlock, 0, sizeof(LessonBlock));

            currentBlock->type = get_block_type(blockName);

            continue;
        }

        /* =================================================
           FIELD
           ================================================= */

        if(currentBlock)
        {
            char* eq = strchr(line, '=');

            if(!eq) continue;

            *eq = '\0';

            char* key   = line;
            char* value = eq + 1;

            set_field(currentBlock, key, value);
        }
    }

    fclose(fp);

    return 1;
}