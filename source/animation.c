#include "animation.h"
#include "gameobj.h"

extern void gameobj_push_changes(GameObj *obj);

#define MAX_ANIMDATA_COUNT 64

void gameobj_update_sprite_facing(GameObj *obj);

AnimationData animationLibrary[MAX_ANIMDATA_COUNT]; 
static int free_anim = 0;



/////////////////
/// ANIM DATA ///
/////////////////

void animdata_init_all()
{
	for(int i = 0; i < MAX_ANIMDATA_COUNT; i++)
	{
		animdata_clear(&animationLibrary[i]);
	}
	free_anim = 0;
}

AnimationData *animdata_create(int tile_start, short tile_offset, u16 frame_ct, int facing_offset)
{
	AnimationData *anim = &animationLibrary[free_anim];
	anim->tile_start = tile_start;
	anim->tile_offset = tile_offset;
	anim->frame_ct = frame_ct;
	anim->facing_offset = facing_offset;
	free_anim++;
	return anim;
}

void animdata_set_info(AnimationData *anim_data, int tile_start, short tile_offset, u16 frame_ct, int facing_offset)
{
	anim_data->tile_start = tile_start;
	anim_data->tile_offset = tile_offset;
	anim_data->frame_ct = frame_ct;
	anim_data->facing_offset = facing_offset;
}

void animdata_clear(AnimationData *anim_data)
{
	anim_data->tile_start = 0;
	anim_data->tile_offset = 0;
	anim_data->facing_offset = 0;
	anim_data->frame_ct = 0;
}



/////////////////
/// ANIMATION ///
/////////////////

Animation *anim_init(Animation *anim, AnimationData *anim_data, u8 anim_flags)
{
	if(anim == NULL)
		return NULL;
	anim->anim_data = anim_data;
	anim->flags = anim_flags;
	anim->cur_frame = 0;
	return anim;
}

Animation *anim_clear(Animation *anim)
{
	if(anim == NULL)
		return NULL;
	anim->anim_data = NULL;
	anim->flags = 0;
	anim->cur_frame = 0;
	return anim;
}


// advance an animation one frame
void anim_update(Animation *anim)
{
	if(anim == NULL)
		return;
	if((anim->flags & ANIM_FLAG_PLAYING) && anim->anim_data->frame_ct > 0)
	{
		int next_frame;
		if(anim->flags & ANIM_FLAG_REVERSED)
			next_frame = (anim->cur_frame + anim->anim_data->frame_ct - 1) % anim->anim_data->frame_ct;
		else
			next_frame = (anim->cur_frame + 1) % anim->anim_data->frame_ct;

		// stop after one cycle if not looping
		if(next_frame == 0 && !(anim->flags & ANIM_FLAG_LOOPING))
		{
			anim_stop(anim);
			if((anim->flags & ANIM_FLAG_CLAMP) == 0)
				anim->cur_frame = next_frame;
		}
		else
			anim->cur_frame = next_frame;
	}
}

// play an animation
void anim_play(Animation *anim)
{
	if(anim == NULL)
		return;
	anim->cur_frame = 0;
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// stop playing an animation
void anim_stop(Animation *anim)
{
	if(anim == NULL)
		return;
	anim->flags = anim->flags & ~ANIM_FLAG_PLAYING;
}

// play an animation backwards
void anim_play_reversed(Animation *anim)
{
	if(anim == NULL)
		return;
	anim_set_reversed(anim, true);
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// force an animation to play forwards
void anim_play_forward(Animation *anim)
{
	if(anim == NULL)
		return;
	anim_set_reversed(anim, false);
	anim->flags = anim->flags | ANIM_FLAG_PLAYING;
}

// set animation reverse flag
void anim_set_reversed(Animation *anim, bool reversed)
{
	if(anim == NULL)
		return;
	if(reversed)
		anim->flags |= ANIM_FLAG_REVERSED;
	else
		anim->flags &= ~ANIM_FLAG_REVERSED;
}

