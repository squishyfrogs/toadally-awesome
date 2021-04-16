#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#define OBJ_COUNT 128

typedef struct struct_GameObj {
	int obj_id;
	OBJ_ATTR *attr;

	//u16 attr0;			// AABC DDEE FFFF FFFF || A = SHAPE, B = COLORMODE, C = MOSAIC, D = GFXMODE, E = OBJMODE, F = YPOS
	//u16 attr1;			// AABC xxxD DDDD DDDD || A = SIZE, B = VFLIP, C = HFLIP, D = XPOS || BCxxx = AFFINDEX
	//u16 attr2;			// AAAA BBCC CCCC CCCC || A = PALBANK, B = PRIORITY, C = TILEINDEX

	u32 tile_id;			// index of upperleft tile in obj memory
	u16 pal_bank_id;		// index of palette in pal memory
	u8 priority;			// draw order priority in layer (0 = drawn on top)
	u16 spr_shape;			// shape of sprite
	u16 spr_size;			// size of sprite
	
	int pos_x, pos_y;		// position in screen space

	u16 anim_tile_offset;	// how many tiles to offset by per frame (should be same as sprite size in most cases)
	u16 anim_frame_ct;		// frame count of current animation
	u16 anim_cur_frame;		// current frame of current animation

} GameObj;

#endif //GAMEOBJ_H