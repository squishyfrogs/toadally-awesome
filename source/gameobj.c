#include <string.h>
#include <tonc.h>
#include "gameobj.h"

// sprite includes 
#include "sprites/kirby.h"
#include "sprites/gear.h"
#include "sprites/mario.h"

#define SPRITE_CT 3

void init_objs();
void init_test_spr();
void update_objs();
void update_anim();

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;



OBJ_ATTR *kirby;
int kirby_x= 96, kirby_y= 92;
u32 kirby_tid= 0, kirby_pb= 0;		// tile id, pal-bank

OBJ_ATTR *gear;
int gear_x = 32, gear_y= 32;
u32 gear_tid= (kirbyTilesLen / 32) + 1 , gear_pb= 1;		// tile id, pal-bank

OBJ_ATTR *mario;
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
	kirby = &obj_buffer[0];
	obj_set_attr(kirby, 
		ATTR0_SQUARE,							// Square, regular sprite
		ATTR1_SIZE_32,							// 32x32p, 
		ATTR2_PALBANK(kirby_pb) | kirby_tid);	// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(kirby, kirby_x, kirby_y);

	
	gear = &obj_buffer[1];
	obj_set_attr(gear, 
		ATTR0_SQUARE,							// Square, regular sprite
		ATTR1_SIZE_32,							// 32x32p, 
		ATTR2_PALBANK(gear_pb) | gear_tid);		// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(gear, gear_x, gear_y);
	
	mario = &obj_buffer[2];
	obj_set_attr(mario, 
		ATTR0_SQUARE,							// Square, regular sprite
		ATTR1_SIZE_32,							// 32x32p, 
		ATTR2_PALBANK(mario_pb) | mario_tid);	// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(mario, mario_x, mario_y);

}


void update_objs()
{
	// move left/right
	kirby_x += 2*key_tri_horz();
	gear_x += key_tri_horz();
	// move up/down
	kirby_y += 2*key_tri_vert();
	gear_y += key_tri_vert();

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_L))
	{
		//hide/unhide sprites
		static bool spr_hidden;
		if(spr_hidden)
			obj_unhide_multi(kirby, DCNT_MODE0, SPRITE_CT);
		else
			obj_hide_multi(kirby, SPRITE_CT);
		spr_hidden = !spr_hidden;
	}
	

	// flip
	if(key_hit(KEY_A))	// horizontally
		kirby->attr1 ^= ATTR1_HFLIP;
	if(key_hit(KEY_B))	// vertically
		kirby->attr1 ^= ATTR1_VFLIP;
	

	// make it glow (via palette swapping)
	//pb= key_is_down(KEY_SELECT) ? 1 : 0;

	// toggle mapping mode
	//if(key_hit(KEY_START))
	//	REG_DISPCNT ^= DCNT_OBJ_1D;
	
	// Hey look, it's one of them build macros!
	kirby->attr2= ATTR2_BUILD(kirby_tid, kirby_pb, 0);
	obj_set_pos(kirby, kirby_x, kirby_y);

	//REG_BG1HOFS= kirby_x;
	//REG_BG1VOFS= kirby_y;

	gear->attr2= ATTR2_BUILD(gear_tid, gear_pb, 0);
	obj_set_pos(gear, gear_x, gear_y);

	

	mario->attr2= ATTR2_BUILD(mario_tid + (mario_frame * mario_anim_off), mario_pb, 0);
	obj_set_pos(mario, mario_x, mario_y);

	// copy changes into oam memory
	oam_copy(oam_mem, obj_buffer, SPRITE_CT);	// only need to update one
}

void update_anim()
{
	mario_frame = (mario_frame + 1) % mario_frame_ct;	
}