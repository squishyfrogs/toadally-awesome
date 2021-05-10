#include "animation.h"

#define MAX_ANIM_COUNT 64

AnimationData animationLibrary[MAX_ANIM_COUNT]; 
static int free_anim = 0;



void animations_init()
{
	for(int i = 0; i < MAX_ANIM_COUNT; i++)
	{
		clear_animation(&animationLibrary[i]);
	}
	free_anim = 0;
}


AnimationData *anim_create(short tile_offset, u16 frame_ct, u8 flags)
{
	AnimationData *anim = &animationLibrary[free_anim];
	anim->tile_offset = tile_offset;
	anim->frame_ct = frame_ct;
	anim->flags = flags;
	anim->cur_frame = 0;
	free_anim++;
	return anim;
}

void anim_set_info(AnimationData *anim, short tile_offset, u16 frame_ct, u8 flags)
{
	anim->tile_offset = tile_offset;
	anim->frame_ct = frame_ct;
	anim->flags = flags;
	anim->cur_frame = 0;
}

void clear_animation(AnimationData *anim)
{
	anim->tile_offset = 0;
	anim->frame_ct = 0;
	anim->flags = 0;
	anim->cur_frame = 0;
}



// advance an animation one frame
void anim_update(AnimationData *anim)
{
	if((anim->flags & ANIM_FLAG_PLAYING) && anim->frame_ct > 0)
	{
		if(anim->flags & ANIM_FLAG_REVERSED)
			anim->cur_frame = (anim->cur_frame + anim->frame_ct - 1) % anim->frame_ct;
		else
			anim->cur_frame = (anim->cur_frame + 1) % anim->frame_ct;

		// stop after one cycle if not looping
		if(anim->cur_frame == 0 && !(anim->flags & ANIM_FLAG_LOOPING))
			anim_stop(anim);
	}
}

// play an animation
void anim_play(AnimationData *anim)
{
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// stop playing an animation
void anim_stop(AnimationData *anim)
{
	anim->flags = anim->flags & ~ANIM_FLAG_PLAYING;
}

// play an animation backwards
void anim_play_reversed(AnimationData *anim)
{
	anim_set_reversed(anim, true);
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// force an animation to play forwards
void anim_play_forward(AnimationData *anim)
{
	anim_set_reversed(anim, false);
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// set animation reverse flag
void anim_set_reversed(AnimationData *anim, bool reversed)
{
	if(reversed)
		anim->flags |= ANIM_FLAG_REVERSED;
	else
		anim->flags &= ~ANIM_FLAG_REVERSED;
}