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
	u16 offset = 256;
	// Load map into SBB 30
	for(int i = 0; i < 1024; i++)
	{
		se_mem[30][i] = map[i] + offset;
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
		return 0;
	return (x%MAP_SIZE_X)+(y*MAP_SIZE_X);
}

// get the collision info of a given tile
ushort get_tile_col_info(int pos_x, int pos_y)
{
	if(pos_x < 0 || pos_y < 0 || pos_x >= MAP_SIZE_X || pos_y >= MAP_SIZE_Y)
		return 0;
	return map_collision_info[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)];
}

// get the contents of a given tile, or NULL if tile is empty
GameObj *get_tile_contents(int pos_x, int pos_y)
{
	if(pos_x < 0 || pos_y < 0 || pos_x >= MAP_SIZE_X || pos_y >= MAP_SIZE_Y)
		return NULL;
	return map_contents[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)];
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents(GameObj *obj, int pos_x, int pos_y)
{
	if(pos_x < 0 || pos_y < 0 || pos_x >= MAP_SIZE_X || pos_y >= MAP_SIZE_Y)
		return false;
	if(map_contents[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)] != NULL)
		return false;
	
	map_contents[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)] = obj;
	return true;
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(struct struct_GameObj *obj, int tile_id)
{
	if(map_contents[tile_id] != NULL)
		return false;

	map_contents[tile_id] = obj;
	return true;
}

// clear the contents of a given tile and free it for use
void clear_tile_contents(int pos_x, int pos_y)
{
	if(pos_x < 0 || pos_y < 0 || pos_x >= MAP_SIZE_X || pos_y >= MAP_SIZE_Y)
		return;
	map_contents[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)] = NULL;
}

// clear the contents of a given tile and free it for use
void clear_tile_contents_by_id(int tile_id)
{
	map_contents[tile_id] = NULL;
}
