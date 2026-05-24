/* =========================================================
   FILE: lesson.h
   ========================================================= */

#ifndef LESSON_H
#define LESSON_H

#define MAX_BLOCKS 256
#define MAX_STR    256

typedef enum
{
    BLOCK_NONE,

    BLOCK_DIALOG,
    BLOCK_TERM,
    BLOCK_COMPARE,
    BLOCK_EXAMPLE,
    BLOCK_SEQUENCE,
    BLOCK_GRAMMAR_BOX,
    BLOCK_PARTICLE,
    BLOCK_BREAKDOWN,
    BLOCK_QUIZ,
    BLOCK_SUMMARY,
    BLOCK_SFX,
    BLOCK_REWARD,
    BLOCK_UNLOCK

} BlockType;

typedef struct
{
    BlockType type;

    char field1[MAX_STR];
    char field2[MAX_STR];
    char field3[MAX_STR];
    char field4[MAX_STR];
    char field5[MAX_STR];
    char field6[MAX_STR];
    char field7[MAX_STR];
    char field8[MAX_STR];

} LessonBlock;

/* ---------------------------------------------------------
   DIALOG
   field1=portrait  field2=name  field3=text
   --------------------------------------------------------- */
#define DIALOG_PORTRAIT  field1
#define DIALOG_NAME      field2
#define DIALOG_TEXT      field3

/* ---------------------------------------------------------
   TERM
   field1=word  field2=meaning
   --------------------------------------------------------- */
#define TERM_WORD        field1
#define TERM_MEANING     field2

/* ---------------------------------------------------------
   COMPARE
   field1=left_title  field2=left  field3=right_title  field4=right
   --------------------------------------------------------- */
#define COMPARE_LEFT_TITLE   field1
#define COMPARE_LEFT         field2
#define COMPARE_RIGHT_TITLE  field3
#define COMPARE_RIGHT        field4

/* ---------------------------------------------------------
   EXAMPLE
   field1=jp  field2=romaji  field3=es
   --------------------------------------------------------- */
#define EXAMPLE_JP       field1
#define EXAMPLE_ROMAJI   field2
#define EXAMPLE_ES       field3

/* ---------------------------------------------------------
   SEQUENCE
   field1=step
   --------------------------------------------------------- */
#define SEQUENCE_STEP    field1

/* ---------------------------------------------------------
   GRAMMAR_BOX
   field1=title  field2=content
   --------------------------------------------------------- */
#define GBOX_TITLE       field1
#define GBOX_CONTENT     field2

/* ---------------------------------------------------------
   PARTICLE
   field1=symbol  field2=name  field3=function
   --------------------------------------------------------- */
#define PARTICLE_SYMBOL    field1
#define PARTICLE_NAME      field2
#define PARTICLE_FUNCTION  field3

/* ---------------------------------------------------------
   BREAKDOWN
   field1=jp
   field2=line1     field3=line1_es
   field4=line2     field5=line2_es
   field6=line3     field7=line3_es
   --------------------------------------------------------- */
#define BREAKDOWN_JP        field1
#define BREAKDOWN_LINE1     field2
#define BREAKDOWN_LINE1_ES  field3
#define BREAKDOWN_LINE2     field4
#define BREAKDOWN_LINE2_ES  field5
#define BREAKDOWN_LINE3     field6
#define BREAKDOWN_LINE3_ES  field7

/* ---------------------------------------------------------
   QUIZ
   field1=question
   field2=a  field3=b  field4=c
   field5=correct  field6=success_text  field7=fail_text
   --------------------------------------------------------- */
#define QUIZ_QUESTION      field1
#define QUIZ_A             field2
#define QUIZ_B             field3
#define QUIZ_C             field4
#define QUIZ_CORRECT       field5
#define QUIZ_SUCCESS_TEXT  field6
#define QUIZ_FAIL_TEXT     field7

/* ---------------------------------------------------------
   SUMMARY
   field1=line1  field2=line2  field3=line3
   --------------------------------------------------------- */
#define SUMMARY_LINE1    field1
#define SUMMARY_LINE2    field2
#define SUMMARY_LINE3    field3

/* ---------------------------------------------------------
   SFX
   field1=id
   --------------------------------------------------------- */
#define SFX_ID           field1

/* ---------------------------------------------------------
   REWARD
   field1=type  field2=value
   --------------------------------------------------------- */
#define REWARD_TYPE      field1
#define REWARD_VALUE     field2

/* ---------------------------------------------------------
   UNLOCK
   field1=lesson
   --------------------------------------------------------- */
#define UNLOCK_LESSON    field1

/* ========================================================= */

typedef struct
{
    char lessonId[64];
    char title[64];
    char category[64];
    char background[64];

    int totalBlocks;

    LessonBlock blocks[MAX_BLOCKS];

} Lesson;

typedef enum {
    PORTRAIT_NEUTRAL  = 0,
    PORTRAIT_HAPPY    = 1,
    PORTRAIT_THINKING = 2,
    PORTRAIT_TRIUNFANTE = 3,
    PORTRAIT_THINKING2 = 4,

} PortraitID;

int lesson_load(const char* filename, Lesson* lesson);

#endif