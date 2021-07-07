#include <tonc.h>
#include "pausemenu.h"
#include "input.h"

void create_textbox(uint bgnr, int left, int top, int right, int bottom, uint opacity);
void close_textbox();


void pausemenu_init()
{
	
}


void pausemenu_open()
{
	input_lock(INPLCK_PAUSE);
	create_textbox(0, 172, 8, 232, 92, 8);
	
}


void pausemenu_close()
{
	close_textbox();
	input_unlock(INPLCK_PAUSE);
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