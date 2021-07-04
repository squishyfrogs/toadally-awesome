#include <tonc.h>
#include "colors.h"
#include "regmem.h"
#include "sprites/greencyclepal.h"

#define COLOR_CT 16
static int pal_id = 0;
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
	pal_id = mem_load_palette(greenCyclePal);
}

// cycle color 
void color_cycle_update()
{
	clr = (clr+1) % 16;

	//pal_obj_bank[2][1] = pal_obj_bank[pal_id][clr];

}
