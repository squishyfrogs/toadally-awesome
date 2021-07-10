
//{{BLOCK(map3overlay)

//======================================================================
//
//	map3overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 320 + 2048 = 2880
//
//	Time-stamp: 2021-07-09, 23:17:33
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP3OVERLAY_H
#define GRIT_MAP3OVERLAY_H

#define map3overlayTilesLen 320
extern const unsigned short map3overlayTiles[160];

#define map3overlayMapLen 2048
extern const unsigned short map3overlayMap[1024];

#define map3overlayPalLen 512
extern const unsigned short map3overlayPal[256];

#endif // GRIT_MAP3OVERLAY_H

//}}BLOCK(map3overlay)
