#ifndef HTS_ENGINE_API_H
#define HTS_ENGINE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define TRUE  1
#define FALSE 0

typedef int HTS_Boolean;

// ==============================
// Forward declarations
// ==============================

typedef struct _HTS_Engine HTS_Engine;

// ==============================
// Engine struct (simplificado)
// ==============================

struct _HTS_Engine {
    int sampling_rate;
    int fperiod;

    int nsamples;
    short *speech; // salida PCM
};

// ==============================
// API básica
// ==============================

// init / free
void HTS_Engine_initialize(HTS_Engine *engine);
void HTS_Engine_clear(HTS_Engine *engine);

// carga modelo
HTS_Boolean HTS_Engine_load(HTS_Engine *engine, const char **voices, int num_voices);

// configuración
void HTS_Engine_set_sampling_rate(HTS_Engine *engine, int i);
void HTS_Engine_set_fperiod(HTS_Engine *engine, int i);

// síntesis
HTS_Boolean HTS_Engine_synthesize_from_strings(
    HTS_Engine *engine,
    const char **lines,
    int num_lines
);

// salida
int HTS_Engine_get_nsamples(HTS_Engine *engine);
short* HTS_Engine_get_generated_speech(HTS_Engine *engine);

#ifdef __cplusplus
}
#endif

#endif