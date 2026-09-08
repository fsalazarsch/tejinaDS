#ifndef SCENE_H
#define SCENE_H

typedef enum {
    SCENE_MENU,
    SCENE_TABLA_HIRAGANA,
    SCENE_TEST_KANA,
    SCENE_GRAMMAR_001,
    SCENE_GRAMMAR_002,
    SCENE_GRAMMAR_003,
    

} SceneID;

extern SceneID currentScene;

extern char currentLessonPath[256];

#endif