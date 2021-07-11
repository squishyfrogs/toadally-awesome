
//{{BLOCK(map6overlay)

//======================================================================
//
//	map6overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 320 + 2048 = 2880
//
//	Time-stamp: 2021-07-11, 15:45:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP6OVERLAY_H
#define GRIT_MAP6OVERLAY_H

#define map6overlayTilesLen 320
extern const unsigned short map6overlayTiles[160];

#define map6overlayMapLen 2048
extern const unsigned short map6overlayMap[1024];

#define map6overlayPalLen 512
extern const unsigned short map6overlayPal[256];

#endif // GRIT_MAP6OVERLAY_H

//}}BLOCK(map6overlay)
