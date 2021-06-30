#include <string.h>
#include <tonc.h>
#include "map.h"
#include "gameobj.h"


#define MAP_PAL_LEN 512 //2 bytes per color, 256 color

typedef struct MapTile_T{
	unsigned short height;
	unsigned short col_info;
	GameObj *tile_contents;
	GameObj *floor_contents;
} MapTile;




void map_tile_clear(MapTile *tile);
void map_tile_clear_contents(MapTile *tile);
void map_clear_col_info();
void map_clear_contents();



// Map Data
static MapData current_map;
MapTile map_tiles[MAP_SIZE];

// collision data for map
//unsigned short map_collision_info[MAP_SIZE];
// gameobj data
//GameObj *map_contents[MAP_SIZE];




////////////////
/// Map Data ///
////////////////


// initiation function
void map_init()
{
	map_clear();
}


void set_map_data(const unsigned short *palette, const unsigned short *tiles, int tile_len, const unsigned short *map, int map_len, const unsigned short *col_info)
{
	current_map.palette = palette;
	current_map.tiles = tiles;
	current_map.tile_len = tile_len;
	current_map.map = map;
	current_map.map_len = map_len;
	current_map.col_info = col_info;
}

void load_map_from_current_data()
{
	load_map_from_data(&current_map);
}

void load_map_from_data(MapData *map_data)
{
	// Load palette
	load_map_palette(map_data->palette);
	
	// load tiles
	load_map_tiles(map_data->tiles, map_data->tile_len);
	
	// create the map out of those tiles
	load_map(map_data->map, map_data->map_len);

	// load collision info
	load_map_col_info(map_data->col_info);
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
	//memcpy(map_collision_info, map_col, sizeof(unsigned short)*MAP_SIZE);
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map_tiles[i].col_info = map_col[i];
	}
}

// clear map collision data
void map_clear_col_info()
{
	//memset(map_collision_info, 0, sizeof(unsigned short)*MAP_SIZE);
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map_tiles[i].col_info = 0;
	}
}


//clear all aspects of a map
void map_clear()
{
	//map_clear_col_info();
	//map_clear_contents();
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map_tile_clear(&map_tiles[i]);
	}
}

// clear a single tile
void map_tile_clear(MapTile *tile)
{
	tile->height = 0;
	tile->col_info = 0;
	tile->tile_contents = NULL;
	tile->floor_contents = NULL;
}


//clear a map of any gameobj info
void map_clear_contents()
{
	for(int i = 0; i < MAP_SIZE; i++)
	{
		map_tile_clear_contents(&map_tiles[i]);
	}
}

// clear a single tile
void map_tile_clear_contents(MapTile *tile)
{
	tile->tile_contents = NULL;
	tile->floor_contents = NULL;
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
	
	return map_tiles[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)].col_info;
}

// get the contents of a given tile, or NULL if tile is empty
GameObj *get_tile_contents(int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return NULL;
	return map_tiles[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)].tile_contents;
}

// get the floor contents of a given tile, or NULL if tile is empty
GameObj *get_tile_floor_contents(int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return NULL;
	return map_tiles[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)].floor_contents;
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

	return (map_tiles[(tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X)].tile_contents == NULL);
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



// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return false;

	return set_tile_contents_by_id(obj, (tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X));
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return false;

	if(map_tiles[tile_id].tile_contents != NULL)
		return false;

	map_tiles[tile_id].tile_contents = obj;
	return true;
}

// clear the contents of a given tile and free it for use
void remove_tile_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return;
	
	return remove_tile_contents_by_id(obj, (tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X));
}

// clear the contents of a given tile and free it for use
void remove_tile_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return;
	if(map_tiles[tile_id].tile_contents == obj)
		map_tiles[tile_id].tile_contents = NULL;
}



///////////////////
/// Tile Floors ///
///////////////////

// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile_floor(GameObj *obj, int tile_x, int tile_y)
{
	bool success = set_floor_contents(obj, tile_x, tile_y);
	if(success)
		gameobj_set_tile_pos(obj, tile_x, tile_y);
	return success;
}

// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile_floor_by_id(GameObj *obj, int tile_id)
{
	bool success = set_floor_contents_by_id(obj, tile_id);
	if(success)
		gameobj_set_tile_pos_by_id(obj, tile_id);
	return success;
}


// set the contents of a given tile, only succeeds if tile is empty
bool set_floor_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return false;

	return set_floor_contents_by_id(obj, (tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X));
}

// set the contents of a given tile, only succeeds if tile is empty
bool set_floor_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return false;

	if(map_tiles[tile_id].floor_contents != NULL)
		return false;

	map_tiles[tile_id].floor_contents = obj;
	return true;
}

// clear the contents of a given tile and free it for use
void remove_floor_contents(GameObj *obj, int tile_x, int tile_y)
{
	if(tile_x < 0 || tile_y < 0 || tile_x >= MAP_SIZE_X || tile_y >= MAP_SIZE_Y)
		return;
	
	return remove_floor_contents_by_id(obj, (tile_x%MAP_SIZE_X)+(tile_y*MAP_SIZE_X));
}

// clear the contents of a given tile and free it for use
void remove_floor_contents_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return;
	if(map_tiles[tile_id].floor_contents == obj)
		map_tiles[tile_id].floor_contents = NULL;
}
