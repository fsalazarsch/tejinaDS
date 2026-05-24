/* =========================================================
   FILE: renderer.c
   ========================================================= */

#include <citro2d.h>
#include <string.h>
#include <stdio.h>

#include "lesson.h"

/* =========================================================
   SETUP
   ========================================================= */

extern C2D_Font fontdialog;
extern C2D_Font font2;
extern C2D_TextBuf g_dynamicBuf;

extern C2D_SpriteSheet portraits[5];
/* =========================================================
   COLORES
   ========================================================= */

#define COL_WHITE   C2D_Color32(255, 255, 255, 255)
#define COL_YELLOW  C2D_Color32(255, 220,  80, 255)
#define COL_CYAN    C2D_Color32( 80, 220, 255, 255)
#define COL_GREEN   C2D_Color32(100, 230, 100, 255)
#define COL_BLACK   C2D_Color32(0, 0, 0, 255)
#define COL_GRAY    C2D_Color32(180, 180, 180, 255)
#define COL_BG_DARK C2D_Color32( 20,  20,  20, 50)

/* =========================================================
   HELPER
   ========================================================= */

static float portrait_x        = -150.0f;
static float portrait_target_x = -150.0f;
static int   portrait_current   = -1;

static int   typewriter_len   = 0;
static int   typewriter_timer = 0;
static char  typewriter_buf[512];


static void draw_text_f(const char* str,
                        float x, float y,
                        float sz, u32 color,
                        C2D_Font fnt)
{
    if (!str || str[0] == '\0') return;
    C2D_Text t;
    C2D_TextFontParse(&t, fnt, g_dynamicBuf, str);
    C2D_TextOptimize(&t);
    C2D_DrawText(&t, C2D_WithColor, x, y, 0.5f, sz, sz, color);
}

static void draw_text(const char* str,
                      float x, float y,
                      float sz, u32 color)
{
    if(!str || str[0] == '\0') return;

    C2D_Text t;
    C2D_TextFontParse(&t, fontdialog, g_dynamicBuf, str);
    C2D_TextOptimize(&t);
    C2D_DrawText(&t, C2D_WithColor, x, y, 0.5f, sz, sz, color);
}

static void draw_typewriter(float x, float y, float sz, u32 color)
{
    char visible[512];
    strncpy(visible, typewriter_buf, typewriter_len);
    visible[typewriter_len] = '\0';
    draw_text_f(visible, x, y, sz, color, fontdialog);
}

static int portrait_index(const char* name) {
    if (strcmp(name, "neutral")   == 0) return PORTRAIT_NEUTRAL;
    if (strcmp(name, "happy")     == 0) return PORTRAIT_HAPPY;
    if (strcmp(name, "thinking") == 0) return PORTRAIT_THINKING;
    if (strcmp(name, "triunfante") == 0) return PORTRAIT_TRIUNFANTE;
    return 0; // neutral por defecto
}

void portrait_set(int idx)
{
    portrait_current = idx;
    if (idx >= 0)
        portrait_target_x = 5.0f;   // posición final en pantalla
    else
        portrait_target_x = -150.0f; // fuera de pantalla
}

void typewriter_set(const char* text)
{
    strncpy(typewriter_buf, text, 511);
    typewriter_len   = 0;
    typewriter_timer = 0;
}

/* =========================================================
   RENDER POR TIPO
   ========================================================= */

/* ---------------------------------------------------------
   DIALOG
   portrait / name / text
   --------------------------------------------------------- */
static void render_dialog(LessonBlock* b)
{
    //dibujar a  mai acá
    /* --- Dibujar portrait --- */
    //int portrait_current = portrait_index(b->DIALOG_PORTRAIT);


    if (strcmp(typewriter_buf, b->DIALOG_TEXT) != 0)
    {
        typewriter_set(b->DIALOG_TEXT);
        portrait_set(portrait_index(b->DIALOG_PORTRAIT));
    }
    

    C2D_DrawRectSolid(3, 157, 1.0f, 393, 3, COL_BLACK);
    draw_text_f(b->DIALOG_NAME, 10, 10, 0.6f, COL_YELLOW, fontdialog);

    draw_typewriter(10, 170, 0.9f, COL_BLACK);
    C2D_DrawRectSolid(3, 160, 1.0f, 393, 78, COL_BG_DARK);
     
     
   
    
}

/* ---------------------------------------------------------
   TERM
   word / meaning
   --------------------------------------------------------- */
static void render_term(LessonBlock* b)
{
    //portrait_set(-1);

    char msg[MAX_STR *2];
    snprintf(msg, sizeof(msg), "%s\n%s", b->TERM_WORD, b->TERM_MEANING);
    strncpy(b->DIALOG_TEXT, msg, MAX_STR*2 - 1);
    b->DIALOG_TEXT[MAX_STR - 1] = '\0';


    if (strcmp(typewriter_buf, b->DIALOG_TEXT) != 0)
    {
        typewriter_set(b->DIALOG_TEXT);
        portrait_set(PORTRAIT_THINKING2);
    }
    

    C2D_DrawRectSolid(3, 157, 1.0f, 393, 3, COL_BLACK);
    draw_text_f(b->DIALOG_NAME, 10, 10, 0.6f, COL_YELLOW, fontdialog);

    draw_typewriter(10, 170, 0.9f, COL_BLACK);
    C2D_DrawRectSolid(3, 160, 1.0f, 393, 78, COL_BG_DARK);

    //draw_text_f(b->TERM_WORD,  10, 20, 0.8f, COL_CYAN, font2);
    //draw_text(b->TERM_MEANING, 10, 70, 0.5f, COL_BLACK);
}

/* ---------------------------------------------------------
   COMPARE
   left_title / left / right_title / right
   --------------------------------------------------------- */
static void render_compare(LessonBlock* b)
{
    draw_text(b->COMPARE_LEFT_TITLE,  10,  10, 0.5f, COL_GRAY);
    draw_text(b->COMPARE_LEFT,        10,  38, 0.55f, COL_BLACK);

    draw_text(b->COMPARE_RIGHT_TITLE, 170, 10, 0.5f, COL_GRAY);
    draw_text(b->COMPARE_RIGHT,       170, 38, 0.55f, COL_YELLOW);
}

/* ---------------------------------------------------------
   EXAMPLE
   jp / romaji / es
   --------------------------------------------------------- */
static void render_example(LessonBlock* b)
{
    draw_text(b->EXAMPLE_JP,     10, 10, 0.65f, COL_CYAN);
    draw_text(b->EXAMPLE_ROMAJI, 10, 50, 0.5f,  COL_GRAY);
    draw_text(b->EXAMPLE_ES,     10, 78, 0.5f,  COL_WHITE);
}

/* ---------------------------------------------------------
   SEQUENCE
   step
   --------------------------------------------------------- */
static void render_sequence(LessonBlock* b)
{
    draw_text(b->SEQUENCE_STEP, 10, 50, 0.65f, COL_GREEN);
}

/* ---------------------------------------------------------
   GRAMMAR_BOX
   title / content
   --------------------------------------------------------- */
static void render_grammar_box(LessonBlock* b)
{
    C2D_DrawRectSolid(5, 5, 0.4f, 310, 90, COL_BG_DARK);
    draw_text(b->GBOX_TITLE,   14, 14, 0.55f, COL_YELLOW);
    draw_text(b->GBOX_CONTENT, 14, 48, 0.5f,  COL_WHITE);
}

/* ---------------------------------------------------------
   PARTICLE
   symbol / name / function
   --------------------------------------------------------- */
static void render_particle(LessonBlock* b)
{
    draw_text(b->PARTICLE_SYMBOL,   10, 10, 0.9f, COL_YELLOW);
    draw_text(b->PARTICLE_NAME,     10, 60, 0.5f, COL_GRAY);
    draw_text(b->PARTICLE_FUNCTION, 10, 88, 0.5f, COL_WHITE);
}

/* ---------------------------------------------------------
   BREAKDOWN
   jp / line1 + line1_es / line2 + line2_es / line3 + line3_es
   --------------------------------------------------------- */
static void render_breakdown(LessonBlock* b)
{
    draw_text(b->BREAKDOWN_JP, 10, 5, 0.55f, COL_CYAN);

    draw_text(b->BREAKDOWN_LINE1,    10,  38, 0.55f, COL_YELLOW);
    draw_text(b->BREAKDOWN_LINE1_ES, 120, 38, 0.5f,  COL_WHITE);

    draw_text(b->BREAKDOWN_LINE2,    10,  66, 0.55f, COL_YELLOW);
    draw_text(b->BREAKDOWN_LINE2_ES, 120, 66, 0.5f,  COL_WHITE);

    draw_text(b->BREAKDOWN_LINE3,    10,  94, 0.55f, COL_YELLOW);
    draw_text(b->BREAKDOWN_LINE3_ES, 120, 94, 0.5f,  COL_WHITE);
}

/* ---------------------------------------------------------
   QUIZ
   question / a / b / c / correct / success_text / fail_text
   --------------------------------------------------------- */
static void render_quiz(LessonBlock* b)
{
    draw_text(b->QUIZ_QUESTION, 10,  5,  0.5f, COL_WHITE);

    draw_text("A)",             10,  42, 0.5f, COL_GRAY);
    draw_text(b->QUIZ_A,        30,  42, 0.5f, COL_WHITE);

    draw_text("B)",             10,  68, 0.5f, COL_GRAY);
    draw_text(b->QUIZ_B,        30,  68, 0.5f, COL_WHITE);

    draw_text("C)",             10,  94, 0.5f, COL_GRAY);
    draw_text(b->QUIZ_C,        30,  94, 0.5f, COL_WHITE);
}

/* ---------------------------------------------------------
   SUMMARY
   line1 / line2 / line3
   --------------------------------------------------------- */
static void render_summary(LessonBlock* b)
{
    draw_text(b->SUMMARY_LINE1, 10, 20, 0.5f, COL_WHITE);
    draw_text(b->SUMMARY_LINE2, 10, 50, 0.5f, COL_WHITE);
    draw_text(b->SUMMARY_LINE3, 10, 80, 0.5f, COL_WHITE);
}

/* ---------------------------------------------------------
   SFX
   id  (solo debug — no dibuja nada en pantalla)
   --------------------------------------------------------- */
static void render_sfx(LessonBlock* b)
{
    (void)b;
}

/* ---------------------------------------------------------
   REWARD
   type / value
   --------------------------------------------------------- */
static void render_reward(LessonBlock* b)
{
    char msg[MAX_STR *2];
    snprintf(msg, sizeof(msg), "¡Has ganado +%s %s!", b->REWARD_VALUE, b->REWARD_TYPE);
    strncpy(b->DIALOG_TEXT, msg, MAX_STR*2 - 1);
    b->DIALOG_TEXT[MAX_STR - 1] = '\0';


    if (strcmp(typewriter_buf, b->DIALOG_TEXT) != 0)
    {
        typewriter_set(b->DIALOG_TEXT);
        portrait_set(PORTRAIT_TRIUNFANTE);
    }
    

    C2D_DrawRectSolid(3, 157, 1.0f, 393, 3, COL_BLACK);
    draw_text_f(b->DIALOG_NAME, 10, 10, 0.6f, COL_YELLOW, fontdialog);

    draw_typewriter(10, 170, 0.9f, COL_BLACK);
    C2D_DrawRectSolid(3, 160, 1.0f, 393, 78, COL_BG_DARK);




    //draw_text(msg, 80, 60, 0.7f, COL_YELLOW);
}

/* ---------------------------------------------------------
   UNLOCK
   lesson  (solo debug — no dibuja nada en pantalla)
   --------------------------------------------------------- */
static void render_unlock(LessonBlock* b)
{
    (void)b;
}

/* =========================================================
   DISPATCH PRINCIPAL
   Llama esto dentro del frame, despues de C2D_SceneBegin()
   ========================================================= */

void lesson_render_block(Lesson* lesson, int index)
{
    if(index < 0 || index >= lesson->totalBlocks) return;

    C2D_TextBufClear(g_dynamicBuf);

    LessonBlock* b = &lesson->blocks[index];


    // --- PORTRAIT: animar siempre (para que el slide-out funcione aunque cambie bloque)
    portrait_x += (portrait_target_x - portrait_x) * 0.15f;
    if (portrait_current >= 0 && portraits[portrait_current])
    {
        C2D_Image img = C2D_SpriteSheetGetImage(portraits[portrait_current], 0);
        C2D_DrawImageAt(img, portrait_x, 40, 0.5f, NULL, 1.0f, 1.0f);
    }

    // --- TYPEWRITER: avanzar siempre (solo dibuja quien quiera)
    typewriter_timer++;
    if (typewriter_timer >= 2)
    {
        typewriter_timer = 0;
        int total = strlen(typewriter_buf);
        if (typewriter_len < total) typewriter_len++;
    }

    switch(b->type)
    {
        case BLOCK_DIALOG:      render_dialog(b);      break;
        case BLOCK_TERM:        render_term(b);        break;
        case BLOCK_COMPARE:     render_compare(b);     break;
        case BLOCK_EXAMPLE:     render_example(b);     break;
        case BLOCK_SEQUENCE:    render_sequence(b);    break;
        case BLOCK_GRAMMAR_BOX: render_grammar_box(b); break;
        case BLOCK_PARTICLE:    render_particle(b);    break;
        case BLOCK_BREAKDOWN:   render_breakdown(b);   break;
        case BLOCK_QUIZ:        render_quiz(b);        break;
        case BLOCK_SUMMARY:     render_summary(b);     break;
        case BLOCK_SFX:         render_sfx(b);         break;
        case BLOCK_REWARD:      render_reward(b);      break;
        case BLOCK_UNLOCK:      render_unlock(b);      break;
        default:                                       break;
    }
}