#include <string.h>
#include <tonc.h>

#include "layers.h"
#include "game.h"



extern void init_objs();	//temp
extern void init_map();		//temp

extern void gameobj_init_all();
extern void playerobj_init();
extern void ui_init();
extern void playerobj_update();
extern void ui_update();
extern void ui_update_anim();
extern void update_world_pos();

extern void game_update_temp();
extern void gameobj_push_all_updates();
extern void gameobj_update_anim_all();

void reg_init();
void timer_init();

void main_game_loop();
void draw_bg();

void test_init_tte_se4();
void test_run_tte_se4();
void bg_demo();
void update_text_temp();


void win_textbox(uint bgnr, int left, int top, int right, int bottom, uint bldy);

static enum GameState game_state;

int main(void)
{
	game_state = GS_DEBUG;

	irq_init(NULL);
	//irq_add(II_VBLANK, NULL);
	irq_enable(II_VBLANK);
	reg_init(); 
	
	gameobj_init_all();
	playerobj_init();
	ui_init();

	init_objs(); 	
	init_map();

	test_init_tte_se4();
	
	test_run_tte_se4();

	main_game_loop();


	return 0;
}

void main_game_loop()
{
	static uint32_t anim_sync;
	static uint32_t ui_anim_sync;
	while (1) 
	{
		//vid_vsync();		//resource hog
		VBlankIntrWait();	//slower but saves power
		key_poll();

		anim_sync++;
		if(anim_sync > ANIM_SPEED)
		{
			// animation update functions go in here
			gameobj_update_anim_all();
			anim_sync %= ANIM_SPEED;
		}
		ui_anim_sync++;
		if(ui_anim_sync > UI_ANIM_SPEED)
		{
			//ui looked weird at the slower anim speed so it runs on its own cycle
			ui_update_anim();
			ui_anim_sync %= UI_ANIM_SPEED;
		}


		// gameplay update functions go out here
		update_text_temp();
		playerobj_update();				// update player first
		game_update_temp();				// update other gameobjs 
		update_world_pos();				//push the map around
		ui_update();

		// update gameobj attrs based on gameplay changes
		gameobj_push_all_updates();
	}
}

// initialize the bg + obj registers 
void reg_init()
{
	// set up BG0 for text
	REG_BG0CNT= BG_CBB(LAYER_TEXT) | BG_SBB(30);
	// set up BG1 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
	REG_BG1CNT= BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
}




void timer_init()
{
	// Overflow every ~1 second:
	// 0x4000 ticks @ FREQ_1024

	REG_TM2D = -0x4000;				// 111 ticks to overflow - should produce 60hz cycle?
	REG_TM2CNT= TM_FREQ_1024;		// use 1024 cycle timer
	REG_TM2CNT = TM_ENABLE;
	REG_TM3CNT= TM_ENABLE | TM_CASCADE;		// tm1 cascades into tm0
}

void test_init_tte_se4()
{

	// (1) Init for text
//	tte_init_chr4c(
//		0,									// BG number.
//		BG_CBB(0)|BG_SBB(16),				// BG control.
//		0xF000,								// Screen-entry base
//		bytes2word(13,15,15,15),				// Color attributes.
//		CLR_MAG,							// Ink color
//		&verdana9_b4Font,					// Verdana 9, with shade.
//		(fnDrawg)chr4c_drawg_b4cts_fast);	// b4cts renderer, asm version
	

	// --- (1) Base TTE init for tilemaps ---
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		0,						// Tile offset (special cattr)
		CLR_WHITE,				// Ink color
		14,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)


	tte_init_con();

	// --- (2) Init some colors ---
	//pal_bg_bank[1][15]= CLR_RED;
	//pal_bg_bank[2][15]= CLR_GREEN;
	//pal_bg_bank[3][15]= CLR_BLUE;
	//pal_bg_bank[4][15]= CLR_WHITE;
	//pal_bg_bank[5][15]= CLR_MAG;

	// (3) Create and print to a text box.
	//win_textbox(0, 8, 160-32+4, 232, 160-4, 8);

	

}



void win_textbox(uint bgnr, int left, int top, int right, int bottom, uint bldy)
{
	REG_WIN0H= left<<8 | right;
	REG_WIN0V=  top<<8 | bottom;
	REG_WIN0CNT= WIN_ALL | WIN_BLD;
	REG_WINOUTCNT= WIN_ALL;

	REG_BLDCNT= (BLD_ALL&~BIT(bgnr)) | BLD_BLACK;
	REG_BLDY= bldy;

	REG_DISPCNT |= DCNT_WIN0;

	tte_set_margins(left, top, right, bottom);
}


void test_run_tte_se4()
{
//	tte_erase_screen();

	// --- (3) Print some text ---
	//tte_set_margins(16,16,144,144);
	tte_set_pos(8, 8);
	// "Hello world in different colors"
	tte_write("\n Hahahahaha\n");
	tte_write(" #{cx:0x1000}It took me an entire day\n");
	tte_write(" #{cx:0x2000}But I made text and sprites \n show up at the same time\n");
	tte_write(" #{cx:0x5000}It was a 1 line fix....\n");

	// Color use explained
	//tte_set_pos(8, 84);
	//tte_write("#{cx:0x0000}C#{cx:0x1000}o#{cx:0x2000}l");
	//tte_write("#{cx:0x3000}o#{cx:0x4000}r#{cx:0x5000}s");
	//tte_write("#{cx:0} provided by \\#{cx:#}.");

}

void draw_bg()
{

}


void update_text_temp()
{
	if(key_hit(KEY_SELECT))
	{
		static bool txt_hidden;
		if(txt_hidden)
			test_run_tte_se4();
		else
			tte_erase_screen();
		txt_hidden = !txt_hidden;
		//tte_erase_rect(0,0,64,32);
	}
}
