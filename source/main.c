#include <string.h>
#include <tonc.h>
#include "test.h"

#include "tiles.h"	//bg tiles
#include "box.h"	//test sprite
#include "metr.h"	//metroid sprite
#include "kirby.h"	//kirby sprite


extern int testFunction(int test_int);
void main_game_loop();
void init_test_spr();
void play_test_spr();
void init_bg();
void draw_bg();

void test_init_tte_se4();
void test_run_tte_se4();


OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

OBJ_ATTR *kirby;
int kirby_x= 96, kirby_y= 32;
u32 kirby_tid= 0, kirby_pb= 0;		// tile id, pal-bank

int main(void)
{
	irq_init(NULL);
	irq_enable(II_VBLANK);
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
	


	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], kirbyTiles, kirbyTilesLen);
	memcpy(pal_obj_mem, kirbyPal, kirbyPalLen);
	memcpy(&tile_mem[4][32], tilesTiles, tilesTilesLen);
	memcpy(pal_obj_mem + kirbyPalLen, tilesPal, tilesPalLen);
	//memcpy(&tile_mem[4][0], boxTiles, boxTilesLen);
	//memcpy(pal_obj_mem, boxPal, boxPalLen);

	oam_init(obj_buffer, 128);
	//REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;
	
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



void test_init_tte_se4()
{
	
	// --- (1) Base TTE init for tilemaps ---
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		0,						// Tile offset (special cattr)
		CLR_YELLOW,				// Ink color
		14,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)

	// --- (2) Init some colors ---
	pal_bg_bank[1][15]= CLR_RED;
	pal_bg_bank[2][15]= CLR_GREEN;
	pal_bg_bank[3][15]= CLR_BLUE;
	pal_bg_bank[4][15]= CLR_WHITE;
	pal_bg_bank[5][15]= CLR_MAG;

	pal_bg_bank[4][14]= CLR_GRAY;
}

void test_run_tte_se4()
{
	

	// --- (3) Print some text ---
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

void init_bg()
{

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

	

}

void play_test_spr()
{
	// move left/right
	kirby_x += 2*key_tri_horz();
	// move up/down
	kirby_y += 2*key_tri_vert();

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_L))
		test_run_tte_se4();
	if(key_hit(KEY_R))
		tte_erase_screen();

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

	oam_copy(oam_mem, obj_buffer, 1);	// only need to update one
}