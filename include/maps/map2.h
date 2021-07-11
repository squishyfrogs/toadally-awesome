
//{{BLOCK(map2)

//======================================================================
//
//	map2, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 90 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 5760 + 2048 = 8320
//
//	Time-stamp: 2021-07-10, 17:32:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP2_H
#define GRIT_MAP2_H

#define map2TilesLen 5760
extern const unsigned short map2Tiles[2880];

#define map2MapLen 2048
extern const unsigned short map2Map[1024];

#define map2PalLen 512
extern const unsigned short map2Pal[256];

extern const unsigned short map2Col[256];

#endif // GRIT_MAP2_H

//}}BLOCK(map2)
