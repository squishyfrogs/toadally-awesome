
//{{BLOCK(testmap)

//======================================================================
//
//	testmap, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 160 + 2048 = 2240
//
//	Time-stamp: 2021-04-11, 18:29:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TESTMAP_H
#define GRIT_TESTMAP_H

#define testmapTilesLen 160
extern const unsigned short testmapTiles[80];

#define testmapMapLen 2048
extern const unsigned short testmapMap[1024];

#define testmapPalLen 32
extern const unsigned short testmapPal[16];

#endif // GRIT_TESTMAP_H

//}}BLOCK(testmap)
