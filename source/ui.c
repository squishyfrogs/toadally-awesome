#include <tonc.h>
#include "gameobj.h"
#include "animation.h"
#include "layers.h"
#include "sprites/ui/heart.h"
#include "sprites/ui/numbers.h"
#include "sprites/ui/gear.h"
#include "sprites/ui/timegauge.h"

void ui_init();
void ui_start();
void ui_update();

void reset_action_count();
void set_action_count(int count);
void increment_action_counter();
void decrement_action_counter();


int ui_palette;										// shared palette for ui elements
////////////////////
// Action Counter //
////////////////////
#define ACTION_COUNTER_DIGITS		3				// how many digits the action counter takes
#define DIGIT_ANIM_LENGTH			4				// how many frames it takes to roll from one digit to the next
#define ACTION_COUNTER_OFFSET_X		2				// offset from left side of screen
#define ACTION_COUNTER_OFFSET_Y		-2				// offset from bottom of screen
GameObj *action_counter[ACTION_COUNTER_DIGITS];		// action counter objects
int a_tile;											// tile of digit 0 

static int action_count = 0;						// count of actions 
static int count_rolling;							// whether or not to animate the counter changing (and in which direction)
static int c_frame = 0;


////////////////
// Time Gauge //
////////////////
int t_tile;
GameObj *timeGauge;


////////////
// HP Bar //
////////////
#define HP_MAX 6
GameObj *hearts[HP_MAX];	//health bar 
int h_tile;

int player_health = 3;


//////////
// Gear //
//////////

GameObj *gear;
int g_tile;
int g_anim;


// initialize and set up ui
void ui_init()
{
	// TODO: change to uiPal
	ui_palette = mem_load_palette(numbersPal);

	// init action counter
	a_tile = mem_load_tiles(numbersTiles, numbersTilesLen);
	for(int i = 0; i < ACTION_COUNTER_DIGITS; i++)
	{
		action_counter[i] = init_gameobj();
		action_counter[i]->layer_priority = LAYER_OVERLAY;
		int ac_x = ACTION_COUNTER_OFFSET_X + i*8;
		int ac_y = ACTION_COUNTER_OFFSET_Y + 152;
		gameobj_update_attr_full(action_counter[i], ATTR0_SQUARE, ATTR1_SIZE_8x8, ui_palette, a_tile, ac_x, ac_y, OBJPROP_FIXED_POS);
	}
	reset_action_count();


	// init time gauge
	t_tile = mem_load_tiles(timegaugeTiles, timegaugeTilesLen);

	// init hp bar
	h_tile = mem_load_tiles(heartTiles, heartTilesLen);
	for(int i = 0; i < HP_MAX; i++)
	{
		int h_x = 4+i*8;
		int h_y = 0;
		hearts[i] = init_gameobj_full(LAYER_OVERLAY, ATTR0_TALL, ATTR1_SIZE_8x16, ui_palette, h_tile, h_x, h_y, OBJPROP_FIXED_POS);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}


	// init gear
	gear = init_gameobj();
	g_tile = mem_load_tiles(gearTiles, gearTilesLen);
	gear->layer_priority = LAYER_OVERLAY;
	gameobj_update_attr_full(gear, ATTR0_SQUARE, ATTR1_SIZE_32x32, ui_palette, g_tile, 0, 128, OBJPROP_FIXED_POS);
	gameobj_set_anim_info(gear, 3, ANIM_OFFSET_32x32, 0, false);
	g_anim = 1;

}

// prepare ui for gameplay
void ui_start()
{
	reset_action_count();
}

// gameplay update
void ui_update()
{
//	if(key_hit(KEY_R))
//		increment_action_counter();
//	if(key_hit(KEY_L))
//		decrement_action_counter();

	

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

}




void reset_action_count()
{
	action_count = 0;
	for(int i = 0; i < ACTION_COUNTER_DIGITS; i++)
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
	anim_play_forward(gear->anim);
}

void decrement_action_counter()
{
	action_count--;
	count_rolling = -1;
	c_frame = 0;
	anim_play_reversed(gear->anim);
}