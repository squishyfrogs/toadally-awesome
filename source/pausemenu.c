#include <tonc.h>
#include "pausemenu.h"
#include "game.h"
#include "text.h"
#include "input.h"
#include "gameobj.h"
#include "regmem.h"
#include "palettes.h"
#include "sprites/ui/pauseCursor.h"
#include "sprites/ui/menuPanel.h"

void create_textbox(uint bgnr, int left, int top, int right, int bottom, uint opacity);
void close_textbox();

void exit_level();

static bool pausemenu_active;
static bool pause_buffer;				// 1 frame buffer so we don't immediately close the menu upon opening it 
static int selected_action = 0;

#define ACTION_RESUME	0
#define ACTION_QUIT		1
//#define ACTION_RESTART	2

void (*menu_actions[2])();

// pause menu window boundaries
#define PM_L		166
#define PM_R		232
#define PM_TOP		8
#define PM_BOT		48
#define PM_PAD_H	4
#define PM_PAD_V	8
#define PM_LINE_OFF	16


GameObj *pause_cursor;

void pausemenu_init()
{
	int cursorTile = mem_load_tiles(pauseCursorTiles, pauseCursorTilesLen);
	pause_cursor = gameobj_init_full(LAYER_OVERLAY, ATTR0_SQUARE, ATTR1_SIZE_8x8, PAL_ID_UI, cursorTile, 0, 0, OBJPROP_FIXED_POS);
	gameobj_set_sprite_offset(pause_cursor,8,-4);
	selected_action = 0;
	pausemenu_close();
	menu_actions[ACTION_RESUME] = pausemenu_close;
	menu_actions[ACTION_QUIT] = exit_level;
	tte_init_se_no_color(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		1,						// Tile offset (special cattr)
		62,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)
}

void pausemenu_update()
{
	if(!pausemenu_active)
		return;

	if(pause_buffer)
	{
		if(key_hit(KEY_DOWN))
			selected_action = (selected_action+1) % 2;
		if(key_hit(KEY_UP))
			selected_action = ((selected_action-1) + 2) % 2;

		gameobj_set_pixel_pos(pause_cursor, PM_L, PM_TOP + 4 + (selected_action * 16));

		if(key_hit(KEY_ACCEPT))
			menu_actions[selected_action]();
	}
	pause_buffer = true;
}


void pausemenu_open()
{
	tte_init_se_no_color(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		1,						// Tile offset (special cattr)
		62,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)
	input_lock(INPLCK_PAUSE);
	create_textbox(0, PM_L, PM_TOP, PM_R, PM_BOT, 8);
	tte_erase_line();
	
	tte_set_margins(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V, PM_R-PM_PAD_H, PM_BOT-PM_PAD_V);
	tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V);
	tte_write("Resume");
	tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V + PM_LINE_OFF);
	tte_write("Quit");
	//tte_write("Restart");
	//tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V + 2*PM_LINE_OFF);
	selected_action = 0;
	gameobj_set_pixel_pos(pause_cursor, PM_L, PM_TOP + 4 + (selected_action * 16));
	
	gameobj_unhide(pause_cursor);
	pausemenu_active = true;
	pause_buffer = false;
}


void pausemenu_close()
{
	close_textbox();
	tte_erase_rect(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V, PM_R-PM_PAD_H, PM_BOT-PM_PAD_V);
	input_unlock(INPLCK_PAUSE);
	gameobj_hide(pause_cursor);
	pausemenu_active = false;
	pause_buffer = false;
}

bool check_pausemenu_active()
{
	return pausemenu_active;
}

void create_textbox(uint bgnr, int left, int top, int right, int bottom, uint opacity)
{
	REG_WIN0H = left<<8 | right;
	REG_WIN0V = top<<8 | bottom;
	REG_WIN0CNT = WIN_ALL | WIN_BLD;
	REG_WINOUTCNT = WIN_ALL;

	REG_BLDCNT = (BLD_ALL&~BIT(bgnr)) | BLD_BLACK;
	REG_BLDY = opacity;

	REG_DISPCNT |= DCNT_WIN0;

	tte_set_margins(left, top, right, bottom);
}

void close_textbox()
{
	REG_BLDY = 0;
	REG_DISPCNT &= ~DCNT_WIN0;
}

void exit_level()
{
	go_to_game_state(GS_LEVEL_SELECT);
}