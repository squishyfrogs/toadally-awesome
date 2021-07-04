
//{{BLOCK(map1overlay)

//======================================================================
//
//	map1overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 10 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 640 + 2048 = 3200
//
//	Time-stamp: 2021-07-03, 18:59:49
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP1OVERLAY_H
#define GRIT_MAP1OVERLAY_H

#define map1overlayTilesLen 640
extern const unsigned short map1overlayTiles[320];

#define map1overlayMapLen 2048
extern const unsigned short map1overlayMap[1024];

#define map1overlayPalLen 512
extern const unsigned short map1overlayPal[256];

#endif // GRIT_MAP1OVERLAY_H

//}}BLOCK(map1overlay)
