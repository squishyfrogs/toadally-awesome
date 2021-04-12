
//{{BLOCK(gear)

//======================================================================
//
//	gear, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 4x4 
//	Total size: 32 + 160 + 32 = 224
//
//	Time-stamp: 2021-04-11, 22:45:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GEAR_H
#define GRIT_GEAR_H

#define gearTilesLen 160
extern const unsigned short gearTiles[80];

#define gearMapLen 32
extern const unsigned short gearMap[16];

#define gearPalLen 32
extern const unsigned short gearPal[16];

#endif // GRIT_GEAR_H

//}}BLOCK(gear)