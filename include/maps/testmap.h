
//{{BLOCK(testmap)

//======================================================================
//
//	testmap, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 39 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 2496 + 2048 = 5056
//
//	Time-stamp: 2021-04-18, 16:00:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TESTMAP_H
#define GRIT_TESTMAP_H

#define testmapTilesLen 2496
extern const unsigned short testmapTiles[1248];

#define testmapMapLen 2048
extern const unsigned short testmapMap[1024];

#define testmapPalLen 512
extern const unsigned short testmapPal[256];

#endif // GRIT_TESTMAP_H

//}}BLOCK(testmap)
