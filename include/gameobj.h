#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#define OBJ_COUNT 128

typedef struct struct_GameObj {
	int obj_id;
	OBJ_ATTR *attr;
	int pos_x, pos_y;
	u32 tile_id;
	u32 pal_bank_id;
	int priority;
} GameObj;

#endif //GAMEOBJ_H