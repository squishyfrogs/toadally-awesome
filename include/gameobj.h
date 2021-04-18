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
	
	int pos_x, pos_y;		// position in world/screen space
	bool fixed_pos;			// if flag is set, position is relative to screen instead of relative to world

	short anim_tile_offset;	// how many tiles to offset by per frame (should be same as sprite size in most cases) (set negative to reverse animation)
	u16 anim_frame_ct;		// frame count of current animation
	u16 anim_cur_frame;		// current frame of current animation

} GameObj;




GameObj *init_gameobj();
GameObj *init_gameobj_with_id(int obj_id);

int mem_load_palette(const ushort *pal_data);
int mem_load_tiles(const ushort *tile_data, int data_len);


void gameobj_update_attr(GameObj *obj);
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, bool fixed_pos);

void gameobj_set_anim_info(GameObj *obj, u16 frame_count, short tile_offset);

void gameobj_set_pos(GameObj *obj, int x, int y);
void gameobj_change_pos(GameObj *obj, int move_x, int move_y);

void gameobj_update_anim(GameObj *obj);

void gameobj_flip_h(GameObj *obj);
void gameobj_flip_v(GameObj *obj);
void gameobj_set_flip_h(GameObj *obj, bool flip_h);
void gameobj_set_flip_v(GameObj *obj, bool flip_v);
void gameobj_set_flip(GameObj *obj, bool flip_h, bool flip_v);


void gameobj_push_changes(GameObj *obj);

void gameobj_hide_all();
void gameobj_unhide_all();


#endif //GAMEOBJ_H