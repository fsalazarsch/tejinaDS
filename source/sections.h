
#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "themes.h"

#include "nitech_jp_atr503_m001_htsvoice_bin.h"

#include "nds_tts.h"
#include <nds/memory.h>

#define AUDIO_DELAY 2500000

extern ThemeID currentTheme;


NDS_TTS tts;




void run_quiz_hiragana(){
    int bg_main = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 4, 0);
    //consoleSetCursor(NULL, 0, 50);

    

    mmLoadEffect(SFX_TO);
	mmLoadEffect(SFX_KE);
    mmLoadEffect(SFX_I);
    //mm_sfxhand h = mmEffect(SFX_KA);
	//printf("sfx handle: %d", h);
    draw_text_font(bg_main, 20, 80, "行とけい", handle_kana, kana_0Bitmap, themes[currentTheme].kanaText);

   	mmEffect(SFX_TO);
   	swiDelay(AUDIO_DELAY);
   	mmEffect(SFX_KE);
   	swiDelay(AUDIO_DELAY);
   	mmEffect(SFX_I);
   	
    /*soundPlaySample(
    ka_raw_bin,
    SoundFormat_16Bit,
    nature_raw_bin_size,
    11025,
    127,
    64,
    false,
    0
	);
	//mmEffect(SFX_KA);

    //printf("Haz iniciado la opcion run quiz hiragana");

}*/

void run_quiz_katakana(){

    consoleSetCursor(NULL, 0, 50);
    printf("Haz iniciado la opcion run quiz katakana");

}