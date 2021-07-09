
//{{BLOCK(titleScreen)

//======================================================================
//
//	titleScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 553 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 35392 + 2048 = 37952
//
//	Time-stamp: 2021-07-09, 13:07:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLESCREEN_H
#define GRIT_TITLESCREEN_H

#define titleScreenTilesLen 35392
extern const unsigned short titleScreenTiles[17696];

#define titleScreenMapLen 2048
extern const unsigned short titleScreenMap[1024];

#define titleScreenPalLen 512
extern const unsigned short titleScreenPal[256];

#endif // GRIT_TITLESCREEN_H

//}}BLOCK(titleScreen)
