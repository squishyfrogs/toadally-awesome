#ifndef ANIMATION_H
#define ANIMATION_H

#include <tonc.h>

typedef struct struct_AnimationData {
// anim
	int tile_start;			// index of first tile for animation
	short tile_offset;		// how many tiles to offset by per frame (should be same as sprite size in most cases) (set negative to reverse animation)
	int facing_offset;		// how many tiles to offset by per facing direction (set to 0 if unused)
	u16 frame_ct;			// frame count of current animation
}AnimationData;

typedef struct struct_Animation {
	AnimationData *anim_data;		// 
	u8 flags;						// flags for various animation properties
	u16 cur_frame;					// current frame of current animation
}Animation;	


// animation tile offset values
#define ANIM_OFFSET_8x8			1
#define ANIM_OFFSET_8x16		2
#define ANIM_OFFSET_16x16		4
#define ANIM_OFFSET_16x32		8
#define ANIM_OFFSET_32x32		16


// animation flags
#define ANIM_FLAG_PLAYING		0x01
#define ANIM_FLAG_LOOPING		0x02
#define ANIM_FLAG_REVERSED		0x04
#define ANIM_FLAG_ASYMMETRIC	0x08	// whether or not the sprite has unique sprites for East and West facing
#define ANIM_FLAG_CLAMP			0x10	// if set, hold on final frame when animation ends. if not set, return to frame 0


void animdata_init_all();
AnimationData *animdata_create(int tile_start, short tile_offset, u16 frame_ct, int facing_offset);
void animdata_set_info(AnimationData *anim, int tile_start, short tile_offset, u16 frame_ct, int facing_offset);
void animdata_clear(AnimationData *anim);


Animation *anim_init(Animation *anim, AnimationData *anim_data, u8 anim_flags);
Animation *anim_clear(Animation *anim);
void anim_update(Animation *anim);										// advance an animation one frame
void anim_play(Animation *anim);										// play an animation
void anim_stop(Animation *anim);										// stop playing an animation
void anim_play_reversed(Animation *anim);								// play an animation backwards
void anim_play_forward(Animation *anim);								// force an animation to play forwards
void anim_set_reversed(Animation *anim, bool reversed);					// set animation reverse flag

#endif //ANIMATION_H