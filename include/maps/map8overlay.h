
//{{BLOCK(map8overlay)

//======================================================================
//
//	map8overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 13 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 832 + 2048 = 3392
//
//	Time-stamp: 2021-07-11, 17:07:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP8OVERLAY_H
#define GRIT_MAP8OVERLAY_H

#define map8overlayTilesLen 832
extern const unsigned short map8overlayTiles[416];

#define map8overlayMapLen 2048
extern const unsigned short map8overlayMap[1024];

#define map8overlayPalLen 512
extern const unsigned short map8overlayPal[256];

#endif // GRIT_MAP8OVERLAY_H

//}}BLOCK(map8overlay)
