
//{{BLOCK(map8)

//======================================================================
//
//	map8, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 128 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 8192 + 2048 = 10752
//
//	Time-stamp: 2021-07-11, 17:07:13
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP8_H
#define GRIT_MAP8_H

#define map8TilesLen 8192
extern const unsigned short map8Tiles[4096];

#define map8MapLen 2048
extern const unsigned short map8Map[1024];

#define map8PalLen 512
extern const unsigned short map8Pal[256];

extern const unsigned short map8Col[256];

#endif // GRIT_MAP8_H

//}}BLOCK(map8)
