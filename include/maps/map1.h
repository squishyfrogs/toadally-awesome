
//{{BLOCK(map1)

//======================================================================
//
//	map1, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 70 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 4480 + 2048 = 7040
//
//	Time-stamp: 2021-07-05, 17:47:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP1_H
#define GRIT_MAP1_H

#define map1TilesLen 4480
extern const unsigned short map1Tiles[2240];

#define map1MapLen 2048
extern const unsigned short map1Map[1024];

#define map1PalLen 512
extern const unsigned short map1Pal[256];

extern const unsigned short map1Col[256];

#endif // GRIT_MAP1_H

//}}BLOCK(map1)
