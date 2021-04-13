
//{{BLOCK(fe_map)

//======================================================================
//
//	fe_map, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 182 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 11648 + 2048 = 14208
//
//	Time-stamp: 2021-04-12, 22:30:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FE_MAP_H
#define GRIT_FE_MAP_H

#define fe_mapTilesLen 11648
extern const unsigned short fe_mapTiles[5824];

#define fe_mapMapLen 2048
extern const unsigned short fe_mapMap[1024];

#define fe_mapPalLen 512
extern const unsigned short fe_mapPal[256];

#endif // GRIT_FE_MAP_H

//}}BLOCK(fe_map)
