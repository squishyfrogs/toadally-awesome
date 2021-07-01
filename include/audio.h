#ifndef AUDIO_H
#define AUDIO_H

#include "soundbank.h"
#include "soundbank_bin.h"

void audio_init();
void audio_update();

void play_dummy_track();
void play_dummy_sound();

void audio_play_sound(int sound_id);

#endif //AUDIO_H