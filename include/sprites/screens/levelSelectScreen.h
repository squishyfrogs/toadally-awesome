
//{{BLOCK(levelSelectScreen)

//======================================================================
//
//	levelSelectScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 48 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 3072 + 2048 = 5632
//
//	Time-stamp: 2021-06-24, 15:16:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVELSELECTSCREEN_H
#define GRIT_LEVELSELECTSCREEN_H

#define levelSelectScreenTilesLen 3072
extern const unsigned short levelSelectScreenTiles[1536];

#define levelSelectScreenMapLen 2048
extern const unsigned short levelSelectScreenMap[1024];

#define levelSelectScreenPalLen 512
extern const unsigned short levelSelectScreenPal[256];

#endif // GRIT_LEVELSELECTSCREEN_H

//}}BLOCK(levelSelectScreen)
