#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#define OBJ_COUNT 128

typedef struct struct_GameObj {
	int obj_id;
	OBJ_ATTR *attr;
	int pos_x, pos_y;

	//u16 attr0;			// AABC DDEE FFFF FFFF || A = SHAPE, B = COLORMODE, C = MOSAIC, D = GFXMODE, E = OBJMODE, F = YPOS
	//u16 attr1;			// AABC xxxD DDDD DDDD || A = SIZE, B = VFLIP, C = HFLIP, D = XPOS || BCxxx = AFFINDEX
	//u16 attr2;			// AAAA BBCC CCCC CCCC || A = PALBANK, B = PRIORITY, C = TILEINDEX

	u32 tile_id;
	u16 pal_bank_id;
	u8 priority;
	u8 spr_shape;
	u8 spr_size;


} GameObj;

#endif //GAMEOBJ_H