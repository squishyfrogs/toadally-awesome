#include <maxmod.h>
#include "audio.h"
#include "soundbank.h"
#include "soundbank_bin.h"

void audio_init()
{
	// Initialize maxmod with default settings
	// pass soundbank address, and allocate 8 channels.
	mmInitDefault(soundbank_bin, 8);
}


void audio_update()
{
	
}


void play_dummy_track()
{
	//mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
	
}

void play_dummy_sound()
{
	mmEffect( SFX_SWITCH );
}