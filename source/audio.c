#include <maxmod.h>
#include "audio.h"

void audio_init()
{
	// Initialize maxmod with default settings
	// pass soundbank address, and allocate 8 channels.
	mmInitDefault((mm_addr)soundbank_bin, 8);
}


void audio_update()
{
	//test
}


void play_dummy_track()
{
	//mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
	
}

void play_dummy_sound()
{
	mmEffect( SFX_SWITCH );
}

void audio_play_sound(int sound_id)
{
	if(sound_id < 0 || sound_id >= MSL_NSAMPS)
		sound_id = 0;

	mmEffect(sound_id);
}