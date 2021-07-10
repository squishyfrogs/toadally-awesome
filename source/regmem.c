#include <tonc.h>
#include <string.h>
#include "regmem.h"
#include "layers.h"


static int free_tile = 0;		//marker of first free tile in VRAM

////////////////////////
/// MEMORY FUNCTIONS ///
////////////////////////

int mem_load_palette(const ushort *pal_data, int pal_id)
{
	memcpy(pal_obj_mem + (pal_id*16), pal_data, 32);	// 16 colors per palette, 2 bytes per color
	return pal_id;
}

void mem_free_palette(int pal_id)
{
	memset16(pal_obj_mem + (pal_id*16), 0, 32);
}

int mem_load_tiles(const ushort *tile_data, int data_len)
{
	//always block 4? 
	int tile_id = free_tile;
	memcpy(&tile_mem[4][tile_id], tile_data, data_len);
	free_tile += (data_len/32); // divide by 32;
	return tile_id;
}

//void mem_free_tiles(int data_len)


void mem_clear_obj_palettes()
{
	//memset(pal_obj_mem, 0, PAL_OBJ_SIZE);
	memset(pal_obj_mem, 0, 512);
}

void mem_clear_tiles()
{
	//memset(tile_mem, 0, tile)
	free_tile = 0;
}




/////////////////////////
/// REG Set Functions ///
/////////////////////////

// initialize the bg + obj registers
void reg_set_main()
{
	// set up BG0 for text
	REG_BG0CNT = BG_CBB(LAYER_TEXT) | BG_SBB(30);
	// set up BG1 for a 8bpp 32x32t map, using charblock 0 and screenblock 30
	REG_BG1CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	// set up BG2 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
	REG_BG2CNT = BG_PRIO(LAYER_FOREGROUND) | BG_CBB(0) | BG_SBB(28) | BG_8BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
}

// initialize the bg + obj registers for the title screen
void reg_set_title()
{
	REG_BG2CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
	
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG2;

}

// initialize the bg + obj registers for the level select screen
void reg_set_lev_sel()
{
	// set up BG0 for text
	REG_BG0CNT = BG_CBB(LAYER_TEXT) | BG_SBB(30);
	// set up BG1 for a 8bpp 32x32t map, using charblock 0 and screenblock 31
	REG_BG1CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
	//REG_BG2CNT = BG_PRIO(LAYER_BACKGROUND) | BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;
}





