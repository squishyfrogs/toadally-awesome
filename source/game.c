#include <string.h>
#include <tonc.h>
#include "game.h"
#include "gameobj.h"
#include "layers.h"
#include "map.h"
#include "objhistory.h"
#include "sprites.h"
#include "maps.h"




void init_objs();
void init_map();
void game_update_temp();
void set_world_offset(int off_x, int off_y);
void update_world_pos();


int world_offset_x;
int world_offset_y;

GameObj *kirby;
GameObj *mario;
GameObj *kirby2;

GameObj *crate;


void init_objs()
{
	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)

	
	
	// init kirby
	int k_pal = mem_load_palette(kirbyPal);
	int k_tile = mem_load_tiles(kirbyTiles, kirbyTilesLen);
	kirby = init_gameobj_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 96, 96, 0);

	kirby2 = init_gameobj();
	gameobj_update_attr_full(kirby2, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 100, 20, 0);
	
	//set_tile_contents(kirby2, 5,8);
	
	// init mario
	int m_pal = mem_load_palette(marioPal);
	k_tile = mem_load_tiles(marioTiles, marioTilesLen);
	mario = init_gameobj_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_32x32, m_pal, k_tile, 140, 80, 0);
	gameobj_set_anim_info(mario, 4, 16, 0, true);
	anim_play(mario->anim);

	// init crate
	int c_pal = mem_load_palette(cratePal);
	int c_tile = mem_load_tiles(crateTiles, crateTilesLen);
	crate = init_gameobj_full(LAYER_GAMEOBJ, ATTR0_TALL, ATTR1_SIZE_16x32, c_pal, c_tile, 0, 0, OBJPROP_SOLID|OBJPROP_MOVABLE);
	gameobj_set_sprite_offset(crate,0,8);
	place_obj_in_tile(crate, 6, 12);
}

void init_map()
{
	// Load palette
	load_map_palette(testmapPal);
	//load_map_palette(fe_mapPal);
	
	//memcpy(&tile_mem[1][start], fe_mapTiles, fe_mapTilesLen);
	load_map_tiles(fe_mapTiles, fe_mapTilesLen);
	load_map_tiles(testmapTiles, testmapTilesLen);
	
	//load_map(fe_mapMap, fe_mapMapLen);
	load_map(testmapMap, testmapMapLen);
	load_map_col_info(testmapCol);


}



void game_update_temp()
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


// set the world offset values
void set_world_offset(int off_x, int off_y)
{
	world_offset_x = off_x;
	world_offset_y = off_y;
}

// push changes to the world offset (done every frame)
void update_world_pos()
{
	REG_BG1HOFS = world_offset_x;
	REG_BG1VOFS = world_offset_y;
}