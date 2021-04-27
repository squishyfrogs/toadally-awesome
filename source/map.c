#include <string.h>
#include <tonc.h>
#include "map.h"


#define MAP_PAL_LEN 512 //2 bytes per color



unsigned short map_collision_info[MAP_SIZE_X*MAP_SIZE_Y];




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
	memcpy(map_collision_info, map_col, sizeof(unsigned short)*MAP_SIZE_X*MAP_SIZE_Y);
}

// get the collision info of a given tile
ushort get_tile_col_info(int pos_x, int pos_y)
{
	return map_collision_info[(pos_x%MAP_SIZE_X)+(pos_y*MAP_SIZE_X)];
}