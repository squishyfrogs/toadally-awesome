#include <string.h>
#include <tonc.h>

#include "maps.h"
#include "game.h"


#define ANIM_SPEED 5	// 60/speed =  fps 


extern void init_objs();
extern void update_gameobjs();
extern void update_anim();

void reg_init();
void map_init();
void timer_init();

void main_game_loop();
void draw_bg();

void test_init_tte_se4();
void test_run_tte_se4();
void bg_demo();
void update_text();

static enum GameState game_state;

int main(void)
{
	game_state = GS_DEBUG;

	irq_init(NULL);
	//irq_add(II_VBLANK, NULL);
	irq_enable(II_VBLANK);
	reg_init(); 

	init_objs(); 
	
	map_init();

	test_init_tte_se4();
	
	test_run_tte_se4();

	main_game_loop();


	return 0;
}

void main_game_loop()
{
	static uint32_t anim_sync;
	while (1) 
	{
		//vid_vsync();		//resource hog
		VBlankIntrWait();	//slower but saves power
		key_poll();

		anim_sync++;

		if(anim_sync > ANIM_SPEED)
		{
			update_anim();
			anim_sync %= ANIM_SPEED;
		}

		update_text();
		update_gameobjs();

	}
}

// initialize the bg + obj registers 
void reg_init()
{
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30);
	// set up BG1 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
	REG_BG1CNT= BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
}


void map_init()
{
	// Load palette
	memcpy(pal_bg_mem, fe_mapPal, fe_mapPalLen);
	// Load tiles into CBB 0
	int start = 0;	//TODO: make this not a magic number
	u16 offset = 256;
	memcpy(&tile_mem[1][start], fe_mapTiles, fe_mapTilesLen);
	// Load map into SBB 30
	for(int i = 0; i < 1024; i++)
	{
		se_mem[30][i] = fe_mapMap[i] + offset;
	}
	//memcpy(&se_mem[30][1024], fe_mapMap, fe_mapMapLen);
	
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
	
	// --- (1) Base TTE init for tilemaps ---
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		0,						// Tile offset (special cattr)
		CLR_RED,				// Ink color
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
	tte_set_pos(8, 64);
	tte_write("#{cx:0x0000}C#{cx:0x1000}o#{cx:0x2000}l");
	tte_write("#{cx:0x3000}o#{cx:0x4000}r#{cx:0x5000}s");
	tte_write("#{cx:0} provided by \\#{cx:#}.");

}

void draw_bg()
{

}


void update_text()
{
	if(key_hit(KEY_R))
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
