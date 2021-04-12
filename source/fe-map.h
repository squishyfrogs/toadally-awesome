
//{{BLOCK(fe_map)

//======================================================================
//
//	fe_map, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 182 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 16x16 
//	Metatiled by 2x2
//	Total size: 512 + 5824 + 920 + 512 = 7768
//
//	Time-stamp: 2021-04-11, 21:15:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FE_MAP_H
#define GRIT_FE_MAP_H

#define fe_mapTilesLen 5824
extern const unsigned short fe_mapTiles[2912];

#define fe_mapMetaTilesLen 920
extern const unsigned short fe_mapMetaTiles[460];

#define fe_mapMetaMapLen 512
extern const unsigned short fe_mapMetaMap[256];

#define fe_mapPalLen 512
extern const unsigned short fe_mapPal[256];

#endif // GRIT_FE_MAP_H

//}}BLOCK(fe_map)
