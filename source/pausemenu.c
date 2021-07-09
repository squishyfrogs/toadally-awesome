#include <tonc.h>
#include "pausemenu.h"
#include "input.h"

void create_textbox(uint bgnr, int left, int top, int right, int bottom, uint opacity);
void close_textbox();

static bool pausemenu_active;

#define PM_L		166
#define PM_R		232
#define PM_TOP		8
#define PM_BOT		80
#define PM_PAD_H	4
#define PM_PAD_V	8
#define PM_LINE_OFF	16

void pausemenu_init()
{
	pausemenu_close();
}


void pausemenu_open()
{
	input_lock(INPLCK_PAUSE);
	create_textbox(0, PM_L, PM_TOP, PM_R, PM_BOT, 8);
	tte_erase_line();
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		1,						// Tile offset (special cattr)
		CLR_WHITE,				// Ink color
		15,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)
	tte_set_margins(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V, PM_R-PM_PAD_H, PM_BOT-PM_PAD_V);
	tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V);
	tte_write("Resume");
	tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V + PM_LINE_OFF);
	tte_write("Restart");
	tte_set_pos(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V + 2*PM_LINE_OFF);
	tte_write("Quit");
	pausemenu_active = true;
}


void pausemenu_close()
{
	close_textbox();
	tte_erase_rect(PM_L+PM_PAD_H, PM_TOP+PM_PAD_V, PM_R-PM_PAD_H, PM_BOT-PM_PAD_V);
	input_unlock(INPLCK_PAUSE);
	pausemenu_active = false;
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