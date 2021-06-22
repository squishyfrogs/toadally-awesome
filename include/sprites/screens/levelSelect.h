
//{{BLOCK(levelSelect)

//======================================================================
//
//	levelSelect, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 48 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 3072 + 2048 = 5632
//
//	Time-stamp: 2021-06-20, 17:57:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVELSELECT_H
#define GRIT_LEVELSELECT_H

#define levelSelectTilesLen 3072
extern const unsigned short levelSelectTiles[1536];

#define levelSelectMapLen 2048
extern const unsigned short levelSelectMap[1024];

#define levelSelectPalLen 512
extern const unsigned short levelSelectPal[256];

#endif // GRIT_LEVELSELECT_H

//}}BLOCK(levelSelect)
