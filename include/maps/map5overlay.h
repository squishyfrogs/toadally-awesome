
//{{BLOCK(map5overlay)

//======================================================================
//
//	map5overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 320 + 2048 = 2880
//
//	Time-stamp: 2021-07-11, 14:59:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP5OVERLAY_H
#define GRIT_MAP5OVERLAY_H

#define map5overlayTilesLen 320
extern const unsigned short map5overlayTiles[160];

#define map5overlayMapLen 2048
extern const unsigned short map5overlayMap[1024];

#define map5overlayPalLen 512
extern const unsigned short map5overlayPal[256];

#endif // GRIT_MAP5OVERLAY_H

//}}BLOCK(map5overlay)
