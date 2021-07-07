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



void load_level_data(int level_id);													// handles selecting which map data files to access for a given level || located in level.c

void map_init();
void load_map_from_current_data();

void set_map_data(const unsigned short *palette, const unsigned short *tiles, int tile_len, const unsigned short *map, int map_len, const unsigned short *col_info);
void load_map_from_data(MapData *map_data);
void load_map_palette(const unsigned short *map_palette);
void load_map_tiles(const unsigned short *map_tiles, int tiles_len);
void load_map(const unsigned short *map, int map_len);
void load_map_tile_properties(const unsigned short *map_col);
void map_clear_all();																	// clear all aspects of a map

void set_overlay_data(const unsigned short *tiles, int tile_len, const unsigned short *map, int map_len);
void load_overlay_from_data(MapData *overlay_data);
void load_overlay_tiles(const unsigned short *overlay_tiles, int tiles_len);
void load_overlay_map(const unsigned short *overlay_map, int map_len);
void overlay_clear();


/// Tiles ///

#define TILEPROP_SOLID		0x01	// normal terrain
#define TILEPROP_HOLE		0x02	// objs placed onto this tile will fall down
#define TILEPROP_CANGRAB	0x04	// player can latch onto this tile with their tongue (should only be set while also solid, to avoid unintended behavior)
#define TILEPROP_PAIN		0x08	// tile causes damage when stepped on
#define TILEPROP_VICTORY	0x80	// tile that, when stepped on, clears the level

int get_tile_id(int x, int y);														// converts an x,y position to a tile number

ushort get_tile_properties(int tile_x, int tile_y);									// get the collision info of a given tile
bool check_tile_free(int tile_x, int tile_y);										// returns true if the tile is within map bounds and can be entered
struct struct_GameObj *get_tile_contents(int tile_x, int tile_y);					// get the contents of a given tile, or NULL if tile is empty
struct struct_GameObj *get_tile_floor_contents(int tile_x, int tile_y);				// get the contents of a given tile floor, or NULL if tile is empty

bool place_obj_in_tile(struct struct_GameObj *obj, int tile_x, int tile_y);			// sets a tile's contents, and moves the object to that tile
bool place_obj_in_tile_by_id(struct struct_GameObj *obj, int tile_id);				// sets a tile's contents, and moves the object to that tile
bool set_tile_contents(struct struct_GameObj *obj, int tile_x, int tile_y);			// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(struct struct_GameObj *obj, int tile_id);				// set the contents of a given tile, only succeeds if tile is empty
void remove_tile_contents(struct struct_GameObj *obj, int tile_x, int tile_y);		// clear the contents of a given tile and free it for use
void remove_tile_contents_by_id(struct struct_GameObj *obj, int tile_id);			// clear the contents of a given tile and free it for use

bool place_obj_in_tile_floor(struct struct_GameObj *obj, int tile_x, int tile_y);			// sets a tile's floor contents, and moves the object to that tile
bool place_obj_in_tile_floor_by_id(struct struct_GameObj *obj, int tile_id);				// sets a tile's floor contents, and moves the object to that tile
bool set_floor_contents(struct struct_GameObj *obj, int tile_x, int tile_y);		// set the floor contents of a given tile, only succeeds if tile is empty
bool set_floor_contents_by_id(struct struct_GameObj *obj, int tile_id);				// set the floor contents of a given tile, only succeeds if tile is empty
void remove_floor_contents(struct struct_GameObj *obj, int tile_x, int tile_y);		// clear the floor contents of a given tile and free it for use
void remove_floor_contents_by_id(struct struct_GameObj *obj, int tile_id);			// clear the floor contents of a given tile and free it for use


#endif //MAP_H