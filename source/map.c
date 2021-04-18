#include <string.h>
#include <tonc.h>
#include "map.h"


#define MAP_PAL_LEN 512 //2 bytes per color











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