#include "map.h"
#include "maps.h"

extern void set_map_data_temp();


void load_level_data(int level_id)
{
	//set_map_data_temp();
	switch(level_id)
	{
		case 1:
			set_map_data(map1Pal, map1Tiles, map1TilesLen, map1Map, map1MapLen, map1Col);
			set_overlay_data(map1overlayTiles, map1overlayTilesLen, map1overlayMap, map1overlayMapLen);
			break;
		default:
			set_map_data(testmapPal, testmapTiles, testmapTilesLen, testmapMap, testmapMapLen, testmapCol);
			overlay_clear();
			break;
	}

}



////////////
/// Temp ///
////////////


void set_map_data_temp()
{
	// Load palette
//	load_map_palette(testmapPal);
	//load_map_palette(fe_mapPal);
	
	//memcpy(&tile_mem[1][start], fe_mapTiles, fe_mapTilesLen);
	//load_map_tiles(fe_mapTiles, fe_mapTilesLen);
//	load_map_tiles(testmapTiles, testmapTilesLen);
	
	//load_map(fe_mapMap, fe_mapMapLen);
//	load_map(testmapMap, testmapMapLen);
//	load_map_col_info(testmapCol);

	set_map_data(testmapPal, testmapTiles, testmapTilesLen, testmapMap, testmapMapLen, testmapCol);

}