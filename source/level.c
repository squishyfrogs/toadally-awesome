#include "map.h"
#include "maps.h"
#include "objinteract.h"
#include "game.h"
#include "gamedata.h"

extern void playerobj_set_starting_pos(int pos_x, int pos_y);
extern  void set_plaque_state(int plaque_id, int p_state);


#define NUM_LEVELS 16
#define LEVINFO_UNLOCKED	0x01
#define LEVINFO_CLEARED		0x02
volatile static unsigned char level_info[NUM_LEVELS];

static int current_level;


void set_level_data(int level_id)
{
	//set_map_data_temp();
	current_level = level_id - 1;
	switch(level_id)
	{
		case 1:
			set_map_data(level_id, map1Pal, map1Tiles, map1TilesLen, map1Map, map1MapLen, map1Col);
			set_overlay_data(map1overlayTiles, map1overlayTilesLen, map1overlayMap, map1overlayMapLen);
			break;
		case 2:
			set_map_data(level_id, map2Pal, map2Tiles, map2TilesLen, map2Map, map2MapLen, map2Col);
			set_overlay_data(map2overlayTiles, map2overlayTilesLen, map2overlayMap, map2overlayMapLen);
			break;
		case 3:
			set_map_data(level_id, map3Pal, map3Tiles, map3TilesLen, map3Map, map3MapLen, map3Col);
			set_overlay_data(map3overlayTiles, map3overlayTilesLen, map3overlayMap, map3overlayMapLen);
			break;
		default:
			set_map_data(level_id, map1Pal, map1Tiles, map1TilesLen, map1Map, map1MapLen, map1Col);
			set_overlay_data(map1overlayTiles, map1overlayTilesLen, map1overlayMap, map1overlayMapLen);
			//set_map_data(level_id, testmapPal, testmapTiles, testmapTilesLen, testmapMap, testmapMapLen, testmapCol);
			//overlay_clear();
			break;
	}
	
}

void load_map_objs(int level_id)
{
	int player_start_x = 0;
	int player_start_y = 0;
	switch(level_id)
	{
		case 1:
			player_start_x = 5;
			player_start_y = 9;
			floorobj_create_victory_tile_at_position(10,5);
			break;
		case 2:
			player_start_x = 3;
			player_start_y = 12;
			floorobj_create_victory_tile_at_position(12,3);
			break;
		case 3:
			player_start_x = 4;
			player_start_y = 4;
			floorobj_create_victory_tile_at_position(4,10);
			intobj_create_crate_at_position(11,4);
			break;
		default:
			player_start_x = 10;
			player_start_y = 5;
			floorobj_create_victory_tile_at_position(10,5);
			break;
	}

	playerobj_set_starting_pos(player_start_x, player_start_y);
}


void level_clear()
{
	set_level_cleared(current_level);
	set_level_unlocked(current_level+1);
	save_level_info();
	go_to_game_state(GS_LEVEL_SELECT);
}

void level_restart()
{
	go_to_game_state(GS_MAIN_GAME);
}

void set_level_unlocked(int level_id)
{
	//set_plaque_state(level_id, 2);
	level_info[level_id] |= LEVINFO_UNLOCKED;
}

void set_level_cleared(int level_id)
{
	level_info[level_id] |= LEVINFO_CLEARED;
	//set_plaque_state(level_id, 2);
}


bool check_level_unlocked(int level_id)
{
	return level_info[level_id] & LEVINFO_UNLOCKED;
}

bool check_level_cleared(int level_id)
{
	return level_info[level_id] & LEVINFO_CLEARED;
}



void load_level_info()
{
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		level_info[i] = gamedata_load_byte(i);
	}

	level_info[0] |= LEVINFO_UNLOCKED;
}


void save_level_info()
{
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		gamedata_save_byte(level_info[i], i);
	}
}