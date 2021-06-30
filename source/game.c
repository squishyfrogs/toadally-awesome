#include <string.h>
#include <tonc.h>
#include "game.h"
#include "regmem.h"
#include "vector2.h"
#include "gameobj.h"
#include "layers.h"
#include "map.h"
#include "objhistory.h"
#include "sprites.h"
#include "input.h"
#include "objinteract.h"



void init_objs_temp();
void game_update_main();
void game_update_main_temp();
void set_map_data_temp();

void set_world_offset(int off_x, int off_y);
Vector2 get_world_offset();
void update_world_pos();



int world_offset_x;
int world_offset_y;


void game_update_main()
{

	if(!input_locked())
	{
		if(key_hit(KEY_L))
		{
			history_step_back(1);
		}

		if(key_hit(KEY_R))
		{
			//history_return_to_present();
			history_step_forward(1);
		}
	}

	game_update_main_temp();
}





/////////////////////////////
/// Testing & Placeholder ///
/////////////////////////////

///// temp
GameObj *kirby;
GameObj *mario;
GameObj *kirby2;
GameObj *crate;
//////




void init_objs_temp()
{
	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)

	
	
	// init kirby
	int k_pal = mem_load_palette(kirbyPal);
	int k_tile = mem_load_tiles(kirbyTiles, kirbyTilesLen);
	kirby = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 96, 96, 0);

	kirby2 = gameobj_init();
	gameobj_update_attr_full(kirby2, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 100, 20, 0);
	
	//set_tile_contents(kirby2, 5,8);
	
	// init mario
	int m_pal = mem_load_palette(marioPal);
	k_tile = mem_load_tiles(marioTiles, marioTilesLen);
	mario = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_32x32, m_pal, k_tile, 140, 80, 0);
	AnimationData *mario_anim = animdata_create(k_tile, 16, 4, 0);
	gameobj_set_anim_data(mario, mario_anim, ANIM_FLAG_LOOPING);
	//gameobj_set_anim_info(mario, 4, 16, 0, true);
	gameobj_play_anim(mario);

	// init crate
	intobj_create_crate_at_position(6,12);
	intobj_create_coin_at_position(8,10);
	
	floorobj_create_spikes_at_position(4,11);
}

void game_update_main_temp()
{

	gameobj_change_pixel_pos(kirby, key_tri_horz(), key_tri_vert());


	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_START))
	{
		//hide/unhide sprites
		static bool spr_hidden;
		if(spr_hidden)
			gameobj_unhide_all();
		else
			gameobj_hide_all();
		spr_hidden = !spr_hidden;
	}
	

	// flip
	//if(key_hit(KEY_A))	// horizontally
	//	gameobj_flip_h(kirby);
	if(key_hit(KEY_B))	// vertically
		gameobj_flip_v(kirby);
	
	gameobj_set_flip_h(mario, key_is_down(KEY_A));


	// make it glow (via palette swapping)
	//pb= key_is_down(KEY_SELECT) ? 1 : 0;

	// toggle mapping mode
	//if(key_hit(KEY_START))
	//	REG_DISPCNT ^= DCNT_OBJ_1D;

	//ofsett bg based on movement
	//world_offset_x = kirby->pos_x/2;
	//world_offset_y = kirby->pos_y/2;

	


}


////////////////////
/// World Offset ///
////////////////////

// set the world offset values
void set_world_offset(int off_x, int off_y)
{
	world_offset_x = off_x;
	world_offset_y = off_y;
}

// get the current world offset
Vector2 get_world_offset()
{
	Vector2 v;
	v.x = world_offset_x;
	v.y = world_offset_y;
	return v;
}

// push changes to the world offset (done every frame)
void update_world_pos()
{
	REG_BG1HOFS = world_offset_x;
	REG_BG1VOFS = world_offset_y;
}