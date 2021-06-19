#include <string.h>
#include <tonc.h>
#include "map.h"
#include "gameobj.h"



#define MAP_PAL_LEN 512 //2 bytes per color, 256 color

void map_clear_col_info();
void map_clear_contents();


// collision data for map
unsigned short map_collision_info[MAP_SIZE];
// gameobj data
GameObj *map_contents[MAP_SIZE];





////////////////
/// Map Data ///
////////////////


// initiation function
void map_init()
{
	map_clear();
}

// load a palette into palbg memory
void load_map_palette(const ushort *map_palette)
{
	memcpy(pal_bg_mem, map_palette, MAP_PAL_LEN);
}

// load a map into memory
void load_map_tiles(const ushort *map_tiles, int tiles_len)
{
	// Load tiles into CBB 0
	memcpy(&tile_mem[1][0], map_tiles, tiles_len);

}

// load a map into memory
void load_map(const ushort *map, int map_len)
{
	// offset by 256 for reasons
	u16 map_mem_offset = 256;
	// divide len by 2 for reasons
	int half_len = map_len / 2;
	// Load map into SBB 30
	for(int i = 0; i < half_len; i++)
	{
		se_mem[30][i] = map[i] + map_mem_offset;
	}
}

// load map collision data into memory
void load_map_col_info(const unsigned short *map_col)
{
	memcpy(map_collision_info, map_col, sizeof(unsigned short)*MAP_SIZE);
}

// clear map collision data
void map_clear_col_info()
{
	memset(map_collision_info, 0, sizeof(unsigned short)*MAP_SIZE);
}


//clear all aspects of a map
void map_clear()
{
	map_clear_col_info();
	map_clear_contents();
}



//clear a map of any gameobj info
void map_clear_contents()
{
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map_contents[i] = NULL;
	}
}






/////////////////////
/// Tile Contents ///
/////////////////////

// converts an x,y position to a tile number
int get_tile_id(int x, int y)
{
	if(x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y)
		return -1;
	return (x%MAP_SIZE_X)+(y*MAP_SIZE_X);
}

// get the collision info of a given tile
ushort get_tile_col_info(int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return 0;
	return map_collision_info[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)];
}

// get the contents of a given tile, or NULL if tile is empty
GameObj *get_tile_contents(int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return NULL;
	return map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)];
}

// returns true if the tile is within map bounds and can be entered
bool check_tile_free(int tile_x, int tile_y)
{
	// make sure tile is within map bounds
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return false;
	// make sure tile has a valid height
	ushort height = get_tile_col_info(tile_x, tile_y);
	if(height == 0)
		return false;

	return (map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)] == NULL);
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return false;
//	if(map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)] != NULL)
//		return false;
	
	map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)] = obj;
	return true;
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return false;

	if(map_contents[tile_id] != NULL)
		return false;

	map_contents[tile_id] = obj;
	return true;
}

// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile(GameObj *obj, int tile_x, int tile_y)
{
	bool success = set_tile_contents(obj, tile_x, tile_y);
	if(success)
		gameobj_set_tile_pos(obj, tile_x, tile_y);
	return success;
}

// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile_by_id(GameObj *obj, int tile_id)
{
	bool success = set_tile_contents_by_id(obj, tile_id);
	if(success)
		gameobj_set_tile_pos_by_id(obj, tile_id);
	return success;
}

// clear the contents of a given tile and free it for use
void remove_tile_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return;
	if(map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)] == obj)
		map_contents[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)] = NULL;
}

// clear the contents of a given tile and free it for use
void remove_tile_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return;
	if(map_contents[tile_id] == obj)
		map_contents[tile_id] = NULL;
}
