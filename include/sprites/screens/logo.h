
//{{BLOCK(logo)

//======================================================================
//
//	logo, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 71 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 4544 + 2048 = 7104
//
//	Time-stamp: 2021-06-14, 16:31:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LOGO_H
#define GRIT_LOGO_H

#define logoTilesLen 4544
extern const unsigned short logoTiles[2272];

#define logoMapLen 2048
extern const unsigned short logoMap[1024];

#define logoPalLen 512
extern const unsigned short logoPal[256];

#endif // GRIT_LOGO_H

//}}BLOCK(logo)
