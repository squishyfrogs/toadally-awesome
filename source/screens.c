#include <string.h>
#include <tonc.h>

#include "screens.h"
#include "game.h"
#include "layers.h"
#include "audio.h"

#include "sprites/screens/logo.h"
#include "sprites/screens/titleScreen.h"
#include "sprites/screens/levelSelectScreen.h"

// levelselect.c
extern void level_select_init();
extern void level_select_clear();

void load_screen_pal(const ushort *scr_palette);
void load_screen_tiles(const ushort *scr_tiles, int tiles_len);
void load_screen(const ushort *scr, int scr_len);
void unload_screen_tiles(int tiles_len);


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
	unload_screen_tiles(logoTilesLen);
}

void logo_display()
{
	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	load_screen(logoMap, logoMapLen);
	audio_play_sound(SFX_COLLECT_COIN);
}


/////////////
/// Title ///
/////////////

void title_load()
{
	load_screen_pal(titleScreenPal);
	load_screen_tiles(titleScreenTiles, titleScreenTilesLen);
}

void title_unload()
{
	unload_screen_tiles(titleScreenTilesLen);
}

void title_display()
{
	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	load_screen(titleScreenMap, titleScreenMapLen);
}


////////////////////
/// Level Select ///
////////////////////

void lev_sel_load()
{
	load_screen_pal(levelSelectScreenPal);
	load_screen_tiles(levelSelectScreenTiles, levelSelectScreenTilesLen);
}

void lev_sel_unload()
{
	unload_screen_tiles(levelSelectScreenTilesLen);
	level_select_clear();
}

void lev_sel_display()
{
	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;
	load_screen(levelSelectScreenMap, levelSelectScreenMapLen);
	level_select_init();
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

void unload_screen_tiles(int tiles_len)
{
	memset(&tile_mem[1][0], 0, tiles_len);
}

void load_screen(const ushort *scr, int scr_len)
{
	// set up BG2 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
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

void unload_current_screen()
{
	switch (get_game_state())
	{
	case GS_LOGO:
		logo_unload();
		break;
	case GS_TITLE:
		title_unload();
		break;
	case GS_LEVEL_SELECT:
		lev_sel_unload();
		break;
	default:
		break;
	}
}