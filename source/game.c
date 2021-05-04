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
	kirby = init_gameobj(false);
	int k_pal = mem_load_palette(kirbyPal);
	int k_tile = mem_load_tiles(kirbyTiles, kirbyTilesLen);
	gameobj_update_attr_full(kirby, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 96, 96);

	kirby2 = init_gameobj(false);
	gameobj_update_attr_full(kirby2, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 100, 20);
	
	//set_tile_contents(kirby2, 5,8);
	
	// init mario
	mario = init_gameobj(false);
	int m_pal = mem_load_palette(marioPal);
	k_tile = mem_load_tiles(marioTiles, marioTilesLen);
	gameobj_update_attr_full(mario, ATTR0_SQUARE, ATTR1_SIZE_32x32, m_pal, k_tile, 140, 80);
	gameobj_set_anim_info(mario, 4, 16, true);
	gameobj_anim_play(mario);

	// init crate
	crate = init_gameobj(false);
	int c_pal = mem_load_palette(cratePal);
	int c_tile = mem_load_tiles(crateTiles, crateTilesLen);
	gameobj_update_attr_full(crate, ATTR0_TALL, ATTR1_SIZE_16x32, c_pal, c_tile, 0, 0);
	gameobj_set_sprite_offset(crate,0,8);
	gameobj_set_pos_tile(crate, 2,8);
	set_tile_contents(crate, 2, 8);
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

	gameobj_change_pos_pixel(kirby, key_tri_horz(), key_tri_vert());


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

	if(key_hit(KEY_R))
	{
		ObjHistory *oh = get_obj_history(0);
		int tile = get_tile_id_at_time(oh, 4);
		int x = tile % MAP_SIZE_X;
		int y = tile / MAP_SIZE_Y;
		gameobj_set_pos_tile(kirby2, x, y);
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