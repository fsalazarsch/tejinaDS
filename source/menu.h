#ifndef MENU_H
#define MENU_H

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#include <3ds.h>
#include <citro2d.h>
#include "functions.h"
#include "keyboard/keyboard.h"
#include "themes.h"
#include "aprender/escritura/hiragana_katakana/kana_table_screen.h"
#include "aprender/escritura/kanji/kanji_table_screen.h"
#include "tabla.h"
#include "scene.h"

typedef struct {
    float **matriz;
    char **menu;
} MenuResult;

MenuResult display_menu(C2D_TextBuf g_staticBuf,
                        const char* optn,
                        C2D_Font font,
                        C2D_Font font2,
                        C3D_RenderTarget *top,
                        C3D_RenderTarget *bottom);

#endif







/*

Menú Principal
├── 1) Aprender
│   ├── 1.1) Escritura japonesa
│   │   ├── 1.1.1) Hiragana y Katakana
│   │   ├── 1.1.2) Kanji N5/N4
│   │   ├── 1.1.3) Kanji N3
│   │   └── 1.1.4) Kanji N2/N1
│   ├── 1.2) Gramática
│   │   ├── 1.2.1) Estructura de oraciones (SOV)
│   │   ├── 1.2.2) Partículas (は、が、を、に、で...)
│   │   ├── 1.2.3) Verbos y conjugación
│   │   └── 1.2.4) Adjetivos い y な
│   ├── 1.3) Vocabulario
│   │   ├── 1.3.1) Números y tiempo
│   │   ├── 1.3.2) Vida cotidiana
│   │   ├── 1.3.3) Verbos esenciales
│   │   └── 1.3.4) Expresiones básicas
│   └── 1.4) Cultura
│       ├── 1.4.1) Keigo (lenguaje formal)
│       ├── 1.4.2) Costumbres sociales
│       ├── 1.4.3) Anime y manga (contexto)
│       └── 1.4.4) Japón cotidiano
├── 2) Practicar
│   ├── 2.1) Lectura
│   │   ├── 2.1.1) Hiragana y Katakana
│   │   ├── 2.1.2) Frases simples
│   │   ├── 2.1.3) Textos con furigana
│   │   └── 2.1.4) Textos sin furigana
│   ├── 2.2) Escucha
│   │   ├── 2.2.1) Pronunciación básica
│   │   ├── 2.2.2) Diálogos cotidianos
│   │   ├── 2.2.3) Dictado de kana
│   │   └── 2.2.4) Dictado de palabras
│   ├── 2.3) Escritura
│   │   ├── 2.3.1) Práctica libre hiragana
│   │   ├── 2.3.2) Práctica libre katakana
│   │   ├── 2.3.3) Práctica libre kanji
│   │   └── 2.3.4) Crear frases
│   └── 2.4) Conversación
│       ├── 2.4.1) Saludos y presentaciones
│       ├── 2.4.2) Compras y restaurantes
│       ├── 2.4.3) Transporte y viajes
│       └── 2.4.4) Situaciones de emergencia
├── 3) Quiz
│   ├── 3.1) Evaluación inicial
│   ├── 3.2) Por nivel JLPT
│   │   ├── 3.2.1) N5 - Principiante
│   │   ├── 3.2.2) N4 - Básico
│   │   ├── 3.2.3) N3 - Intermedio
│   │   └── 3.2.4) N2/N1 - Avanzado
│   ├── 3.3) Temático
│   │   ├── 3.3.1) Gramática
│   │   ├── 3.3.2) Vocabulario
│   │   ├── 3.3.3) Kanji
│   │   └── 3.3.4) Lectura de frases
│   └── 3.4) Progreso y estadísticas
│       ├── 3.4.1) Resumen de evaluaciones
│       ├── 3.4.2) Puntos fuertes y débiles
│       ├── 3.4.3) Objetivos sugeridos
│       └── 3.4.4) Historial
└── 4) Configuración
    ├── 4.1) Apariencia
    │   ├── 4.1.1) Tema claro/oscuro
    │   ├── 4.1.2) Tamaño de texto
    │   ├── 4.1.3) Idioma de interfaz
    │   └── 4.1.4) Animaciones
    ├── 4.2) Preferencias de estudio
    │   ├── 4.2.1) Mostrar romaji/furigana
    │   └── 4.2.2) Modo quiz kanji
    │           ├── Kun'yomi
    │           ├── On'yomi
    │           ├── Significado
    │           ├── Voz (TTS)
    │           └── Variado
    ├── 4.3) Audio
    │   ├── 4.3.1) Velocidad de voz
    │   ├── 4.3.2) Tono de voz
    │   ├── 4.3.3) Activar/desactivar TTS
    │   └── 4.3.4) Volumen
    └── 4.4) Información
        ├── 4.4.1) Créditos
        ├── 4.4.2) Manual de usuario
        ├── 4.4.3) Reportar errores
        └── 4.4.4) Versión y actualizaciones

=================MENU ANTIGUO=======================



Menú Principal
├── 1) Quiz
│   ├── 1.1) Evaluación inicial
│   │   ├── 1.1.1) Reconocer Hiragana
│   │   ├── 1.1.2) Reconocer Katakana
│   │   ├── 1.1.3) Reconocer Kanji básicos
│   │   └── 1.1.4) Vocabulario inicial
│   ├── 1.2) Simulación NOKEN
│   │   ├── 1.2.1) Nivel N5 (básico)
│   │   ├── 1.2.2) Nivel N4 (intermedio)
│   │   ├── 1.2.3) Nivel N3 (avanzado)
│   │   └── 1.2.4) Nivel N2/N1 (experto)
│   ├── 1.3) Quiz temático
│   │   ├── 1.3.1) Gramática (partículas y estructuras)
│   │   ├── 1.3.2) Vocabulario por tema
│   │   ├── 1.3.3) Kanji por nivel
│   │   └── 1.3.4) Lectura de frases
│   └── 1.4) Progreso y estadísticas
│       ├── 1.4.1) Resumen de evaluaciones
│       ├── 1.4.2) Puntos fuertes y débiles
│       ├── 1.4.3) Objetivos sugeridos
│       └── 1.4.4) Gráficos comparativos
├── 2) Diccionario
│   ├── 2.1) Hiragana y Katakana
│   │   ├── 2.1.1) Tabla completa
│   │   ├── 2.1.2) Escritura y trazos
│   │   ├── 2.1.3) Práctica de sonidos
│   │   └── 2.1.4) Asociaciones visuales
│   ├── 2.2) Kanji
│   │   ├── 2.2.1) Kanji básicos (N5/N4)
│   │   ├── 2.2.2) Kanji intermedios (N3)
│   │   ├── 2.2.3) Kanji avanzados (N2/N1)
│   │   └── 2.2.4) Herramienta de búsqueda por trazos
│   ├── 2.3) Vocabulario
│   │   ├── 2.3.1) Palabras comunes
│   │   ├── 2.3.2) Categorías (números, colores, etc.)
│   │   ├── 2.3.3) Verbos y adjetivos esenciales
│   │   └── 2.3.4) Expresiones idiomáticas
│   └── 2.4) Frases útiles
│       ├── 2.4.1) Diálogos cotidianos
│       ├── 2.4.2) Frases de supervivencia (viajes)
│       ├── 2.4.3) Honoríficos y formalidades
│       └── 2.4.4) Respuestas rápidas
├── 3) Práctica
│   ├── 3.1) Escritura
│   │   ├── 3.1.1) Práctica de Hiragana
│   │   ├── 3.1.2) Práctica de Katakana
│   │   ├── 3.1.3) Práctica de Kanji
│   │   └── 3.1.4) Creación de frases
│   ├── 3.2) Lectura
│   │   ├── 3.2.1) Hiragana y Katakana
│   │   ├── 3.2.2) Kanji con furigana
│   │   ├── 3.2.3) Lectura de textos básicos
│   │   └── 3.2.4) Lectura de textos avanzados
│   ├── 3.3) Escucha
│   │   ├── 3.3.1) Audios básicos
│   │   ├── 3.3.2) Audios intermedios
│   │   ├── 3.3.3) Audios avanzados
│   │   └── 3.3.4) Dictados
│   └── 3.4) Roleplay
│       ├── 3.4.1) Diálogos básicos
│       ├── 3.4.2) Interacción con NPCs
│       ├── 3.4.3) Escenarios reales (compras, viajes)
│       └── 3.4.4) Evaluación de respuestas
└── 4) Configuración
    ├── 4.1) Idioma
    │   ├── 4.1.1) Español
    │   ├── 4.1.2) Inglés
    │   ├── 4.1.3) Japonés
    │   └── 4.1.4) Personalizado
    ├── 4.2) Preferencias
    │   ├── 4.2.1) Dificultad de quizzes
    │   ├── 4.2.2) Orden de aprendizaje
    │   ├── 4.2.3) Modo oscuro/claro
    │   └── 4.2.4) Recordatorios diarios
    ├── 4.3) Gestión de datos
    │   ├── 4.3.1) Guardar progreso
    │   ├── 4.3.2) Cargar progreso
    │   ├── 4.3.3) Exportar/importar datos
    │   └── 4.3.4) Reiniciar progreso
    └── 4.4) Créditos y soporte
        ├── 4.4.1) Equipo desarrollador
        ├── 4.4.2) Manual del usuario
        ├── 4.4.3) Reportar errores
        └── 4.4.4) Actualizaciones

*/


