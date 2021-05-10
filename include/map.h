#ifndef MAP_H
#define MAP_H

#define MAP_SIZE_X 16
#define MAP_SIZE_Y 16
#define MAP_SIZE MAP_SIZE_X*MAP_SIZE_Y

void load_map_palette(const unsigned short *map_palette);
void load_map_tiles(const unsigned short *map_tiles, int tiles_len);
void load_map(const unsigned short *map, int map_len);
void load_map_col_info(const unsigned short *map_col);

void map_init();
void map_clear();															// clear all aspects of a map


int get_tile_id(int x, int y);												// converts an x,y position to a tile number

ushort get_tile_col_info(int pos_x, int pos_y);								// get the collision info of a given tile
struct struct_GameObj *get_tile_contents(int pos_x, int pos_y);				// get the contents of a given tile, or NULL if tile is empty
bool set_tile_contents(struct struct_GameObj *obj, int pos_x, int pos_y);	// set the contents of a given tile, only succeeds if tile is empty
bool set_tile_contents_by_id(struct struct_GameObj *obj, int tile_id);		// set the contents of a given tile, only succeeds if tile is empty
void clear_tile_contents(int pos_x, int pos_y);								// clear the contents of a given tile and free it for use
void clear_tile_contents_by_id(int tile_id);								// clear the contents of a given tile and free it for use

#endif //MAP_H