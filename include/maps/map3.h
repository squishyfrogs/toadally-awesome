
//{{BLOCK(map3)

//======================================================================
//
//	map3, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 89 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 5696 + 2048 = 8256
//
//	Time-stamp: 2021-07-10, 18:19:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP3_H
#define GRIT_MAP3_H

#define map3TilesLen 5696
extern const unsigned short map3Tiles[2848];

#define map3MapLen 2048
extern const unsigned short map3Map[1024];

#define map3PalLen 512
extern const unsigned short map3Pal[256];

extern const unsigned short map3Col[256];

#endif // GRIT_MAP3_H

//}}BLOCK(map3)
