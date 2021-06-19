#include <string.h>
#include <tonc.h>

#include "screens.h"
#include "layers.h"

#include "sprites/screens/logo.h"
#include "sprites/screens/titlescreen.h"

void load_screen_pal(const ushort *scr_palette);
void load_screen_tiles(const ushort *scr_tiles, int tiles_len);
void load_screen(const ushort *scr, int scr_len);


#define PAL_LEN_256 512

////////////
/// Logo ///
////////////

void logo_load()
{
	load_screen_pal(logoPal);
	load_screen_tiles(logoTiles, logoTilesLen);
}

void logo_unload()
{

}

void logo_display()
{
	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;
	load_screen(logoMap, logoMapLen);
}


/////////////
/// Title ///
/////////////

void title_load()
{
	load_screen_pal(titlescreenPal);
	load_screen_tiles(titlescreenTiles, titlescreenTilesLen);
}

void title_unload()
{

}

void title_display()
{
	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;
	load_screen(titlescreenMap, titlescreenMapLen);
}


//////////////
/// Shared ///
//////////////

void load_screen_pal(const ushort *scr_palette)
{
	memcpy(pal_bg_mem, scr_palette, PAL_LEN_256);
}

void load_screen_tiles(const ushort *scr_tiles, int tiles_len)
{
	// Load tiles into CBB 0
	memcpy(&tile_mem[1][0], scr_tiles, tiles_len);
	
}

void load_screen(const ushort *scr, int scr_len)
{
	// set up BG1 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
//	REG_BG1CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	REG_BG2CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	u16 scr_mem_offset = 256;
	// divide len by 2 for reasons
	int half_len = scr_len / 2;
	// Load map into SBB 30
	for(int i = 0; i < half_len; i++)
	{
		se_mem[30][i] = scr[i] + scr_mem_offset;
	}
}

