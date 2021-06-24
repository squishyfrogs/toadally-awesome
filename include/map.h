#ifndef MAP_H
#define MAP_H

#include <tonc.h>

#define MAP_SIZE_X 16
#define MAP_SIZE_Y 16
#define MAP_SIZE MAP_SIZE_X*MAP_SIZE_Y


typedef struct struct_MapData{
	const unsigned short *palette;
	const unsigned short *tiles;
	int tile_len;
	const unsigned short *map;
	int map_len;
	const unsigned short *col_info;
} MapData;


void load_level_data(int level_id);
void set_map_data(const unsigned short *palette, const unsigned short *tiles, int tile_len, const unsigned short *map, int map_len, const unsigned short *col_info);
void load_map_from_current_data();
void load_map_from_data(MapData *map_data);


void load_map_palette(const unsigned short *map_palette);
void load_map_tiles(const unsigned short *map_tiles, int tiles_len);
void load_map(const unsigned short *map, int map_len);
void load_map_col_info(const unsigned short *map_col);

void map_init();
void map_clear();																	// clear all aspects of a map


int get_tile_id(int x, int y);														// converts an x,y position to a tile number

ushort get_tile_col_info(int tile_x, int tile_y);									// get the collision info of a given tile
struct struct_GameObj *get_tile_contents(int tile_x, int tile_y);					// get the contents of a given tile, or NULL if tile is empty
bool check_tile_free(int tile_x, int tile_y);										// returns true if the tile is within map bounds and can be entered
bool set_tile_contents(struct struct_GameObj *obj, int tile_x, int tile_y);			// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(struct struct_GameObj *obj, int tile_id);				// set the contents of a given tile, only succeeds if tile is empty
bool place_obj_in_tile(struct struct_GameObj *obj, int tile_x, int tile_y);			// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile_by_id(struct struct_GameObj *obj, int tile_id);				// sets a tile's contents, and moves the object to that tile
void remove_tile_contents(struct struct_GameObj *obj, int tile_x, int tile_y);		// clear the contents of a given tile and free it for use
void remove_tile_contents_by_id(struct struct_GameObj *obj, int tile_id);			// clear the contents of a given tile and free it for use



#endif //MAP_H