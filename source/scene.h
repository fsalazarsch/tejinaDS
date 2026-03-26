#ifndef SCENE_H
#define SCENE_H

typedef enum {
    SCENE_MENU,
    SCENE_TABLA_HIRAGANA,
    SCENE_TEST_KANA
} SceneID;

extern SceneID currentScene;

#endif