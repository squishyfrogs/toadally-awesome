#include <tonc.h>
#include "gameobj.h"
#include "layers.h"
#include "sprites/ui/heart.h"
#include "sprites/ui/numbers.h"
#include "sprites/ui/gear.h"

void ui_init();
void ui_update();

void reset_action_count();
void set_action_count(int count);
void increment_action_counter();
void decrement_action_counter();


////////////////////
// Action Counter //
////////////////////
#define ACTION_DIGITS 3
#define DIGIT_ANIM_LENGTH 4		//how many frames to roll from one digit to the next
#define ACTION_COUNTER_OFFSET_X		2
#define ACTION_COUNTER_OFFSET_Y		-2
GameObj *action_counter[ACTION_DIGITS];	//action counter
int a_pal;
int a_tile;					//tile of digit 0 

static int action_count = 0;
static int count_rolling;		//whether or not to animate the counter changing (and in which direction)
static int c_frame = 0;

////////////
// HP Bar //
////////////
#define HP_MAX 6
GameObj *hearts[HP_MAX];	//health bar 
int h_pal;
int h_tile;

int player_health = 3;

//////////
// Gear //
//////////

GameObj *gear;
int g_pal;
int g_tile;
int g_anim;

void ui_init()
{
	// TODO: condense UI palettes into a single complete one

	// init action counter
	a_pal = mem_load_palette(numbersPal);
	a_tile = mem_load_tiles(numbersTiles, numbersTilesLen);
	for(int i = 0; i < ACTION_DIGITS; i++)
	{
		action_counter[i] = init_gameobj(true);
		action_counter[i]->priority = LAYER_OVERLAY;
		int ac_x = ACTION_COUNTER_OFFSET_X + i*8;
		int ac_y = ACTION_COUNTER_OFFSET_Y + 152;
		gameobj_update_attr_full(action_counter[i], ATTR0_SQUARE, ATTR1_SIZE_8x8, a_pal, a_tile, ac_x, ac_y);
	}
	set_action_count(752);	//placeholder

	// init hp bar
	h_pal = mem_load_palette(heartPal);
	h_tile = mem_load_tiles(heartTiles, heartTilesLen);
	for(int i = 0; i < HP_MAX; i++)
	{
		hearts[i] = init_gameobj(true);
		hearts[i]->priority = LAYER_OVERLAY;
		int h_x = 4+i*8;
		int h_y = 0;
		gameobj_update_attr_full(hearts[i], ATTR0_TALL, ATTR1_SIZE_8x16, h_pal, h_tile, h_x, h_y);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}


	// init gear
	gear = init_gameobj(true);
	g_pal = mem_load_palette(gearPal);
	g_tile = mem_load_tiles(gearTiles, gearTilesLen);
	gear->priority = LAYER_OVERLAY;
	gameobj_update_attr_full(gear, ATTR0_SQUARE, ATTR1_SIZE_32x32, g_pal, g_tile, 0, 128);
	gameobj_set_anim_info(gear, 3, ANIM_OFFSET_32x32, false);
	g_anim = 1;
	gameobj_anim_set_reversed(gear, true);

}

// gameplay update
void ui_update()
{
	if(key_hit(KEY_R))
		increment_action_counter();
	if(key_hit(KEY_L))
		decrement_action_counter();

	

	for(int i = 0; i < HP_MAX; i++)
	{
		if(i >= player_health)
			hearts[i]->tile_id = h_tile + 2;
		else
			hearts[i]->tile_id = h_tile;
		gameobj_update_attr(hearts[i]);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}
}


// animation update
void ui_update_anim()
{

	// Action Counter
	if(count_rolling != 0)
	{
		// update ones digit first
		int f_offset = action_counter[2]->tile_id - a_tile;
		f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
		action_counter[2]->tile_id = a_tile + f_offset;
		
		//if rolling up to a 0 or down to a 9
		if(((count_rolling > 0) && (action_count % 10 == 0)) || ((count_rolling < 0) && (action_count % 10 == 9)))
		{
			// update tens digit as well
			f_offset = action_counter[1]->tile_id - a_tile;
			f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
			action_counter[1]->tile_id = a_tile + f_offset;

			//repeat process for hundreds digit
			if(((count_rolling > 0) && (action_count % 100 == 0)) || ((count_rolling < 0) && (action_count % 100 == 99)))
			{
				f_offset = action_counter[0]->tile_id - a_tile;
				f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
				action_counter[0]->tile_id = a_tile + f_offset;
			}
		}

		c_frame++;
		if(c_frame == DIGIT_ANIM_LENGTH)
		{
			c_frame = 0;
			count_rolling = 0;
			set_action_count(action_count);		//should be redundant if everything works properly, but just in case
		}
	}

	// Gear
	if(g_anim != 0)
	{
		//gameobj_update_anim(gear);
	}
}




void reset_action_count()
{
	action_count = 0;
	for(int i = 0; i < ACTION_DIGITS; i++)
	{
		action_counter[i]->tile_id = a_tile;
		gameobj_update_attr(action_counter[i]);
	}
}


void set_action_count(int count)
{
	action_count = count;
	// hundreds digit
	action_counter[0]->tile_id = a_tile + (DIGIT_ANIM_LENGTH * ((count / 100) % 10));
	gameobj_update_attr(action_counter[0]);
	// tens digit
	action_counter[1]->tile_id = a_tile + (DIGIT_ANIM_LENGTH * ((count / 10) % 10));
	gameobj_update_attr(action_counter[1]);
	// ones digit
	action_counter[2]->tile_id = a_tile + (DIGIT_ANIM_LENGTH * (count % 10));
	gameobj_update_attr(action_counter[2]);
}

void increment_action_counter()
{
	action_count++;
	count_rolling = 1;
	c_frame = 0;

	gameobj_anim_play(gear);
}

void decrement_action_counter()
{
	action_count--;
	count_rolling = -1;
	c_frame = 0;
}