
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#define AUDIO_DELAY 2500000

void run_quiz_hiragana(){

    consoleSetCursor(NULL, 0, 50);
    mmLoadEffect(SFX_KYO);
	mmLoadEffect(SFX_NE);
    mmLoadEffect(SFX_N);
    //mm_sfxhand h = mmEffect(SFX_KA);
	//printf("sfx handle: %d", h);
    //speak("とけい");
   

   	mmEffect(SFX_KYO);
   	swiDelay(AUDIO_DELAY);
   	mmEffect(SFX_NE);
   	swiDelay(AUDIO_DELAY);
   	mmEffect(SFX_N);
   	
    /*soundPlaySample(
    ka_raw_bin,
    SoundFormat_16Bit,
    nature_raw_bin_size,
    11025,
    127,
    64,
    false,
    0
	);*/
	//mmEffect(SFX_KA);

    printf("Haz iniciado la opcion run quiz hiragana");

}

void run_quiz_katakana(){

    consoleSetCursor(NULL, 0, 50);
    printf("Haz iniciado la opcion run quiz katakana");

}