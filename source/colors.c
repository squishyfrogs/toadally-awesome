#include <tonc.h>
#include "colors.h"
#include "regmem.h"
#include "sprites/greencyclepal.h"

#define COLOR_CT 16
static int pal_id = 0;
static int clr = 0;

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
