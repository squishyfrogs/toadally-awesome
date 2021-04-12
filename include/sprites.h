//======================================================================
//
//	metr, 64x64@4, 
//	+ palette 32 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 64 + 2048 = 2112
//
//	Time-stamp: 2005-12-24, 17:36:57
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __METR__
#define __METR__

#define metrPalLen 64
extern const unsigned int metrPal[16];

#define metrTilesLen 2048
extern const unsigned int metrTiles[512];

#endif // __METR__

//======================================================================
//
//	metr_box, 64x64@4, 
//	+ 64 tiles not compressed
//	Total size: 2048 = 2048
//
//	Time-stamp: 2005-12-24, 17:37:08
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __METR_BOX__
#define __METR_BOX__

#define metr_boxTilesLen 2048
extern const unsigned int metr_boxTiles[512];

#endif // __METR_BOX__


//{{BLOCK(box)

//======================================================================
//
//	box, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2021-04-09, 23:50:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BOX_H
#define GRIT_BOX_H

#define boxTilesLen 512
extern const unsigned int boxTiles[128];

#define boxPalLen 32
extern const unsigned int boxPal[8];

#endif // GRIT_BOX_H

//}}BLOCK(box)

//{{BLOCK(kirby)

//======================================================================
//
//	kirby, 128x16@4, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2021-04-10, 18:44:31
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_KIRBY_H
#define GRIT_KIRBY_H

#define kirbyTilesLen 1024
extern const unsigned short kirbyTiles[512];

#define kirbyPalLen 32
extern const unsigned short kirbyPal[16];

#endif // GRIT_KIRBY_H

//}}BLOCK(kirby)

//{{BLOCK(tiles)

//======================================================================
//
//	tiles, 64x64@4, 
//	+ palette 256 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 512 + 2048 = 2560
//
//	Time-stamp: 2021-04-10, 17:26:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILES_H
#define GRIT_TILES_H

#define tilesTilesLen 2048
extern const unsigned short tilesTiles[1024];

#define tilesPalLen 512
extern const unsigned short tilesPal[256];

#endif // GRIT_TILES_H

//}}BLOCK(tiles)
