#include "map.h"
#include "maps.h"
#include "objinteract.h"
#include "game.h"
#include "gamedata.h"

extern void set_map_data_temp();


void load_level_data(int level_id)
{
	//set_map_data_temp();
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
			set_map_data(level_id, testmapPal, testmapTiles, testmapTilesLen, testmapMap, testmapMapLen, testmapCol);
			overlay_clear();
			break;
	}
	
}


void load_map_objs(int level_id)
{

	switch(level_id)
	{
		case 3:
			intobj_create_crate_at_position(10,8);
			break;
		default:
			break;
	}
}


void level_clear()
{
	gamedata_save();
	go_to_game_state(GS_LEVEL_SELECT);
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
//	load_map_tile_properties(testmapCol);

	set_map_data(0, testmapPal, testmapTiles, testmapTilesLen, testmapMap, testmapMapLen, testmapCol);

}