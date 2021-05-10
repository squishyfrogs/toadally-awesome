#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "layers.h"
#include "animation.h"

#define OBJ_COUNT 128



typedef struct struct_GameObj {
	int obj_id;
	OBJ_ATTR *attr;

	//u16 attr0;				// AABC DDEE FFFF FFFF || A = SHAPE, B = COLORMODE, C = MOSAIC, D = GFXMODE, E = OBJMODE, F = YPOS
	//u16 attr1;				// AABC xxxD DDDD DDDD || A = SIZE, B = VFLIP, C = HFLIP, D = XPOS || BCxxx = AFFINDEX
	//u16 attr2;				// AAAA BBCC CCCC CCCC || A = PALBANK, B = PRIORITY, C = TILEINDEX

	u32 tile_id;				// index of upperleft tile in obj memory
	u16 pal_bank_id;			// index of palette in pal memory
	u8 layer_priority;			// draw order layer_priority in layer (0 = drawn on top)
	u16 spr_shape;				// shape of sprite
	u16 spr_size;				// size of sprite
	
	int pos_x, pos_y;			// position in world/screen space (pixels)
	int spr_off_x, spr_off_y;	// offset from top left pixel of sprite to top left corner of its position

	u16 obj_properties;			// flags for various gameplay-related properties of a given gameobj

	// anim
	AnimationData *anim;		// animation data
	//short anim_tile_offset;		// how many tiles to offset by per frame (should be same as sprite size in most cases) (set negative to reverse animation)
	//u16 anim_frame_ct;			// frame count of current animation
	//u16 anim_cur_frame;			// current frame of current animation
	//u8 anim_flags;				// flags for various animation properties

	//bool play_anim;			// whether or not the animation is playing
	//bool anim_looping;		// whether or not the animation loops on completion

} GameObj;

/////////////////////////
/// Object Properties ///
/////////////////////////
#define OBJPROP_SOLID			0x0001		// does the object take up a tile, or can the player/another object step on it?
#define OBJPROP_PICKUP			0x0002		// does the object get destroyed when the player eats/steps on it?
#define OBJPROP_MOVABLE			0x0004		// can the player push this object by walking into it?
#define OBJPROP_EDIBLE			0x0008		// can the frog consume this?

#define OBJPROP_TIME_IMMUNITY	0x1000		// grants immunity to time-based shenanigans
#define OBJPROP_FIXED_POS		0x2000		// does the object remain in a fixed position on screen? (mostly for UI elements)

inline u16 objprop_is_time_immune(GameObj *obj)
{
	return (obj->obj_properties & OBJPROP_TIME_IMMUNITY);
};

// the final two bits are reserved and refer to the direction the GameObj is facing (see direction.h for specific values)
//
/////////////////////////

GameObj *init_gameobj();
GameObj *init_gameobj_with_id(int obj_id);
GameObj *init_gameobj_full(u16 layer_priority, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, u16 properties);
GameObj *init_gameobj_with_id_full(int obj_id, u16 layer_priority, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, u16 properties);


int mem_load_palette(const ushort *pal_data);
int mem_load_tiles(const ushort *tile_data, int data_len);


void gameobj_update_attr(GameObj *obj);
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, u16 properties);

void gameobj_set_property_flags(GameObj *obj, u16 properties);
void gameobj_set_anim_info(GameObj *obj, u16 frame_count, short tile_offset, bool looping);

void gameobj_set_sprite_offset(GameObj *obj, int x, int y);
void gameobj_set_tile_pos(GameObj *obj, int x, int y);
void gameobj_set_tile_pos_by_id(GameObj *obj, int tile_id);
void gameobj_set_pixel_pos(GameObj *obj, int x, int y);
void gameobj_change_pixel_pos(GameObj *obj, int move_x, int move_y);

void gameobj_set_facing(GameObj *obj, int facing);
int gameobj_get_facing(GameObj *obj);



void gameobj_flip_h(GameObj *obj);
void gameobj_flip_v(GameObj *obj);
void gameobj_set_flip(GameObj *obj, bool flip_h, bool flip_v);
void gameobj_set_flip_h(GameObj *obj, bool flip_h);
void gameobj_set_flip_v(GameObj *obj, bool flip_v);


void gameobj_hide_all();
void gameobj_unhide_all();



#endif //GAMEOBJ_H