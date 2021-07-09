
//{{BLOCK(map2overlay)

//======================================================================
//
//	map2overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 320 + 2048 = 2880
//
//	Time-stamp: 2021-07-08, 16:54:59
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP2OVERLAY_H
#define GRIT_MAP2OVERLAY_H

#define map2overlayTilesLen 320
extern const unsigned short map2overlayTiles[160];

#define map2overlayMapLen 2048
extern const unsigned short map2overlayMap[1024];

#define map2overlayPalLen 512
extern const unsigned short map2overlayPal[256];

#endif // GRIT_MAP2OVERLAY_H

//}}BLOCK(map2overlay)
