#include <string.h>
#include <tonc.h>
#include "gameobj.h"
#include "sprites.h"
// sprite includes 

#define SPRITE_CT 3

void init_objs();
void init_test_spr();


GameObj *init_gameobj();
GameObj *init_gameobj_with_id(int obj_id);

void update_gameobjs();
void update_anim();

void gameobj_update_attr(GameObj *obj);
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tileid, int x, int y);
void gameobj_set_pos(GameObj *obj, int x, int y);
void gameobj_update(GameObj *obj);


GameObj obj_list[OBJ_COUNT];
OBJ_ATTR obj_buffer[OBJ_COUNT];
static int free_obj = 0;		//marker of free obj in the object list


GameObj *kirby;
int kirby_x= 96, kirby_y= 92;
u32 kirby_tid= 0, kirby_pb= 0;		// tile id, pal-bank

GameObj *gear;
int gear_x = 0, gear_y = 128;
u32 gear_tid= 32, gear_pb = 1;		// tile id, pal-bank

GameObj *mario;
int mario_x = 16, mario_y= 64;
u32 mario_tid= 64 , mario_pb= 2;		// tile id, pal-bank
u16 mario_anim_off = 16, mario_frame_ct = 4;
u16 mario_frame = 0;

void init_objs()
{
	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], kirbyTiles, kirbyTilesLen);
	memcpy(pal_obj_mem, kirbyPal, kirbyPalLen);
	memcpy(&tile_mem[4][32], gearTiles, gearTilesLen);
	memcpy(pal_obj_mem + 16, gearPal, gearPalLen);
	memcpy(&tile_mem[4][64], marioTiles, marioTilesLen);
	memcpy(pal_obj_mem + 32, marioPal, marioPalLen);

	//hides all sprites
	oam_init(obj_buffer, 128);

	init_test_spr();
}

void init_test_spr()
{

	// init kirby
	kirby = init_gameobj();
	gameobj_update_attr_full(kirby, ATTR0_SQUARE, ATTR1_SIZE_32x32, kirby_pb, kirby_tid, kirby_x, kirby_y);

	// init gear
	gear = init_gameobj();
	gameobj_update_attr_full(gear, ATTR0_SQUARE, ATTR1_SIZE_32x32, gear_pb, gear_tid, gear_x, gear_y);
	
	// init mario
	mario = init_gameobj();
	gameobj_update_attr_full(mario, ATTR0_SQUARE, ATTR1_SIZE_32x32, mario_pb, mario_tid, mario_x, mario_y);

}


void update_gameobjs()
{
	
	// move left/right
	kirby_x += 2*key_tri_horz();
	// move up/down
	kirby_y += 2*key_tri_vert();

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_L))
	{
		//hide/unhide sprites
		static bool spr_hidden;
		if(spr_hidden)
			obj_unhide_multi(kirby->attr, DCNT_MODE0, SPRITE_CT);
		else
			obj_hide_multi(kirby->attr, SPRITE_CT);
		spr_hidden = !spr_hidden;
	}
	

	// flip
	if(key_hit(KEY_A))	// horizontally
		kirby->attr->attr1 ^= ATTR1_HFLIP;
	if(key_hit(KEY_B))	// vertically
		kirby->attr->attr1 ^= ATTR1_VFLIP;
	

	// make it glow (via palette swapping)
	//pb= key_is_down(KEY_SELECT) ? 1 : 0;

	// toggle mapping mode
	//if(key_hit(KEY_START))
	//	REG_DISPCNT ^= DCNT_OBJ_1D;
	
	gameobj_update(kirby);
	gameobj_set_pos(kirby, kirby_x, kirby_y);

	//REG_BG1HOFS= kirby_x;
	//REG_BG1VOFS= kirby_y;

	gameobj_update(gear);
	gameobj_set_pos(gear, gear_x, gear_y);

	

	mario->attr->attr2= ATTR2_BUILD(mario_tid + (mario_frame * mario_anim_off), mario_pb, 0);
	gameobj_set_pos(mario, mario_x, mario_y);

	// copy changes into oam memory
	oam_copy(oam_mem, obj_buffer, SPRITE_CT);	// only need to update one
}








void update_anim()
{
	mario_frame = (mario_frame + 1) % mario_frame_ct;	
}






// initialize a GameObj and return it to the caller
GameObj *init_gameobj()
{
	GameObj *obj = init_gameobj_with_id(free_obj);
	// eventually free_obj will be stuck at 127 - this is intended behavior (for the time being)
	if(free_obj+1 < OBJ_COUNT)
		free_obj++;
	return obj;
}

// initialize a blank GameObj with a given ID
GameObj *init_gameobj_with_id(int obj_id)
{
	GameObj *obj = &obj_list[obj_id];
	obj->obj_id = obj_id;
	obj->attr = &obj_buffer[obj_id];
	obj->tile_id = 0;
	obj->pal_bank_id = 0;
	obj->priority = 0;
	obj->spr_shape = ATTR0_SQUARE;
	obj->spr_size = ATTR1_SIZE_16x16;	// assume 16x6 for default bc I really doubt anyone is making 16x8 objects 
	return obj;
}



// set a GameObj's attributes
void gameobj_update_attr(GameObj *obj)
{
	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->tile_id));
}

// set a GameObj's attributes
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tileid, int x, int y)
{
	obj->spr_shape = attr0_shape;
	obj->spr_size = attr1_size;
	obj->pal_bank_id = palbank;
	obj->tile_id = tileid;

	obj_set_attr(obj->attr, attr0_shape, attr1_size, (ATTR2_PALBANK(palbank) | tileid));
	obj_set_pos(obj->attr, x, y);
}

// set a GameObj's position
void gameobj_set_pos(GameObj *obj, int x, int y)
{
	obj_set_pos(obj->attr, x, y);
}

// update a GameObj's position, animation, and palette
void gameobj_update(GameObj *obj)
{
	obj->attr->attr2 = ATTR2_BUILD(
		obj->tile_id,
		obj->pal_bank_id,
		obj->priority
	);
}
