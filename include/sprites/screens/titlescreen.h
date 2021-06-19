
//{{BLOCK(titlescreen)

//======================================================================
//
//	titlescreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 290 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 18560 + 2048 = 21120
//
//	Time-stamp: 2021-06-19, 16:28:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLESCREEN_H
#define GRIT_TITLESCREEN_H

#define titlescreenTilesLen 18560
extern const unsigned short titlescreenTiles[9280];

#define titlescreenMapLen 2048
extern const unsigned short titlescreenMap[1024];

#define titlescreenPalLen 512
extern const unsigned short titlescreenPal[256];

#endif // GRIT_TITLESCREEN_H

//}}BLOCK(titlescreen)
