#ifndef AUDIO_H
#define AUDIO_H

#include "soundbank.h"

void audio_init();
void audio_update();
void audio_stop_all();

void audio_play_track(int track_id);
void audio_stop_track();
void audio_play_sound(int sound_id);
void audio_stop_sounds();

#endif //AUDIO_H