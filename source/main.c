#include <string.h>
#include <tonc.h>
#include "test.h"

#include "sprites/kirby.h"
#include "sprites/gear.h"
#include "sprites/box.h"

#include "maps/testmap.h"
#include "maps/fe-map.h"



extern int testFunction(int test_int);
void main_game_loop();
void init_test_spr();
void play_test_spr();
void init_bg();
void draw_bg();

void test_init_tte_se4();
void test_run_tte_se4();
void bg_demo();

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

OBJ_ATTR *kirby;
int kirby_x= 96, kirby_y= 92;
u32 kirby_tid= 0, kirby_pb= 0;		// tile id, pal-bank

OBJ_ATTR *gear;
int gear_x = 32, gear_y= 32;
u32 gear_tid= (kirbyTilesLen / 32) + 1 , gear_pb= 1;		// tile id, pal-bank


int main(void)
{

	irq_init(NULL);
	irq_enable(II_VBLANK);
	// set up BG0 for a 4bpp 32x32t map, using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30);
	REG_BG1CNT= BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
	
	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], kirbyTiles, kirbyTilesLen);
	memcpy(pal_obj_mem, kirbyPal, kirbyPalLen);
	memcpy(&tile_mem[4][32], gearTiles, gearTilesLen);
	memcpy(pal_obj_mem + 16, gearPal, gearPalLen);
	//memcpy(&tile_mem[4][0], boxTiles, boxTilesLen);
	//memcpy(pal_obj_mem, boxPal, boxPalLen);

	oam_init(obj_buffer, 128);
	
	init_bg();
	test_init_tte_se4();
	init_test_spr();

	main_game_loop();


	return 0;
}

void main_game_loop()
{
	irq_init(NULL);
	irq_enable(II_VBLANK);
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	test_run_tte_se4();
	while (1) 
	{
		vid_vsync();
		key_poll();
		play_test_spr();

	}
}



void init_bg()
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



void init_test_spr()
{
	kirby = &obj_buffer[0];
	obj_set_attr(kirby, 
		ATTR0_SQUARE,					// Square, regular sprite
		ATTR1_SIZE_32,					// 32x32p, 
		ATTR2_PALBANK(kirby_pb) | kirby_tid);		// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(kirby, kirby_x, kirby_y);

	
	gear = &obj_buffer[1];
	obj_set_attr(gear, 
		ATTR0_SQUARE,					// Square, regular sprite
		ATTR1_SIZE_32,					// 32x32p, 
		ATTR2_PALBANK(gear_pb) | gear_tid);		// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(gear, gear_x, gear_y);

}

void play_test_spr()
{
	// move left/right
	kirby_x += 2*key_tri_horz();
	gear_x += key_tri_horz();
	// move up/down
	kirby_y += 2*key_tri_vert();
	gear_y += key_tri_vert();

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_L))
	{
		//hide/unhide sprites
		static bool spr_hidden;
		if(spr_hidden)
			obj_unhide_multi(kirby, DCNT_MODE0, 2);
		else
			obj_hide_multi(kirby, 2);
		spr_hidden = !spr_hidden;
	}
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

	// flip
	if(key_hit(KEY_A))	// horizontally
		kirby->attr1 ^= ATTR1_HFLIP;
	if(key_hit(KEY_B))	// vertically
		kirby->attr1 ^= ATTR1_VFLIP;
	

	// make it glow (via palette swapping)
	//pb= key_is_down(KEY_SELECT) ? 1 : 0;

	// toggle mapping mode
	//if(key_hit(KEY_START))
	//	REG_DISPCNT ^= DCNT_OBJ_1D;
	
	// Hey look, it's one of them build macros!
	kirby->attr2= ATTR2_BUILD(kirby_tid, kirby_pb, 0);
	obj_set_pos(kirby, kirby_x, kirby_y);

	//REG_BG1HOFS= kirby_x;
	//REG_BG1VOFS= kirby_y;

	gear->attr2= ATTR2_BUILD(gear_tid, gear_pb, 0);
	obj_set_pos(gear, gear_x, gear_y);

	oam_copy(oam_mem, obj_buffer, 2);	// only need to update one
}