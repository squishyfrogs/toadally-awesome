
//{{BLOCK(map4overlay)

//======================================================================
//
//	map4overlay, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 320 + 2048 = 2880
//
//	Time-stamp: 2021-07-11, 13:05:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAP4OVERLAY_H
#define GRIT_MAP4OVERLAY_H

#define map4overlayTilesLen 320
extern const unsigned short map4overlayTiles[160];

#define map4overlayMapLen 2048
extern const unsigned short map4overlayMap[1024];

#define map4overlayPalLen 512
extern const unsigned short map4overlayPal[256];

#endif // GRIT_MAP4OVERLAY_H

//}}BLOCK(map4overlay)
