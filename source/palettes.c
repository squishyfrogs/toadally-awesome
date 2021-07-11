#include <tonc.h>
#include <string.h>
#include "palettes.h"
#include "regmem.h"
#include "sprites/greencyclepal.h"

// TODO: replace these with dedicated palette headers 
#include "sprites/player/spr_player.h"
#include "sprites/ui/timegauge.h"
#include "sprites/objects/victory_tile.h"

int get_free_pal_id();
void swap_8_colors(int pal_id);
void swap_palettes(int id_a, int id_b);
void swap_bg_pal();


static int used_pals = 0;		//each bit corresponds to whether one of the 16 available sprite palettes is used or not


void load_level_select_palettes()
{

}

void load_main_game_palettes()
{
	palette_load(spr_playerPal, PAL_ID_PLAYER);
	palette_load(timegaugePal, PAL_ID_UI);
	palette_load(victory_tilePal, PAL_ID_OBJS);
}



////////////////
/// Palettes ///
////////////////

int palette_load(const unsigned short *pal_data, int pal_slot_id)
{
	if(pal_slot_id < 0 || pal_slot_id >= 16)
		return -1;
	used_pals |= (1<<pal_slot_id);
	return mem_load_palette(pal_data, pal_slot_id);
}

int palette_load_simple(const unsigned short *pal_data)
{
	int pal_slot_id = get_free_pal_id();
	return palette_load(pal_data, pal_slot_id);
}

void palette_free(int pal_slot_id)
{
	mem_free_palette(pal_slot_id);
	used_pals &= ~(1<<pal_slot_id);
}


int get_free_pal_id()
{
	for(int i = 0; i < PAL_COUNT; i++)
	{
		if((used_pals & (1<<i)) == 0)
		{
			used_pals |= (1<<i);
			return i;
		}
	}
	// no free palettes
	return -1;
}



//////////////////////////////////
/// Grayscale & Color Swapping ///
//////////////////////////////////

void palette_activate_grayscale_mode()
{
	swap_bg_pal();
	swap_8_colors(PAL_ID_PLAYER);

}

void palette_deactivate_grayscale_mode()
{
	swap_bg_pal();
	swap_8_colors(PAL_ID_PLAYER);
}


void swap_8_colors(int pal_id)
{
	int temp[4];
	memcpy(temp, pal_obj_mem + (pal_id*16), 16);
	memmove(pal_obj_mem + (pal_id*16), pal_obj_mem + (pal_id*16) + 8, 16);
	memcpy(pal_obj_mem + (pal_id*16) + 8, temp, 16);
}

void swap_palettes(int id_a, int id_b)
{
	int temp[8];
	memcpy(temp, pal_obj_mem + (id_a*16), 16);
	memmove(pal_obj_mem + (id_a*16), pal_obj_mem + (id_b*16), 16);
	memcpy(pal_obj_mem + (id_b*16), temp, 16);
}

void swap_bg_pal()
{
	int temp[56];
	memcpy(temp, pal_bg_mem + 16, 256-32);
	memmove(pal_bg_mem + 16, pal_bg_mem + 128 + 16, 256-32);
	memcpy(pal_bg_mem + 128 + 16, temp, 256-32);
}


/// Colors ///


#define COLOR_CT 16
//static int pal_id = 0;
static int clr = 0;



//////////////////////
/// Special Colors ///
//////////////////////

// wire colors
#define CLR_WIRE_INACTIVE_A		0x0a2e58
#define CLR_WIRE_INACTIVE_B		0x091837
#define CLR_WIRE_ACTIVE_R_A		0xff5454
#define CLR_WIRE_ACTIVE_R_B		0xb61818
#define CLR_WIRE_ACTIVE_Y_A		0xb69618
#define CLR_WIRE_ACTIVE_Y_B		0xa4760e
#define CLR_WIRE_ACTIVE_B_A		0x156ef6
#define CLR_WIRE_ACTIVE_B_B		0x0d31cf



void color_cycle_init()
{
	clr = 0;
	palette_load(greenCyclePal, PAL_ID_GREENCYCLE);
	//pal_id = mem_load_palette(greenCyclePal, PAL_ID_GREENCYCLE);
}

// cycle color 
void color_cycle_update()
{
	clr = (clr+1) % 16;

	//pal_obj_bank[2][1] = pal_obj_bank[pal_id][clr];

}