#include <tonc.h>

#include "gameobj.h"
#include "animation.h"
#include "layers.h"
#include "sprites/ui/heart.h"
#include "sprites/ui/numbers.h"
#include "sprites/ui/gear.h"
#include "sprites/ui/timegauge.h"


// input.c
extern void input_lock_ui();
extern void input_unlock_ui();


void ui_init();
void ui_start();
void ui_update();

void reset_action_count();
void set_action_count_immediate(int count);
void set_action_count(int count);
void increment_action_counter();
void decrement_action_counter();
void ui_animate_gear_forward();
void ui_animate_gear_backward();


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


static int true_action_count = 0;					// the actual action count, for gameplay purposes
static int displayed_action_count = 0;				// the displayed action count, which will roll up or down until reaching the true action count
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
		action_counter[i] = gameobj_init();
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
		hearts[i] = gameobj_init_full(LAYER_OVERLAY, ATTR0_TALL, ATTR1_SIZE_8x16, ui_palette, h_tile, h_x, h_y, OBJPROP_FIXED_POS);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}


	// init gear
	gear = gameobj_init();
	g_tile = mem_load_tiles(gearTiles, gearTilesLen);
	gear->layer_priority = LAYER_OVERLAY;
	gameobj_update_attr_full(gear, ATTR0_SQUARE, ATTR1_SIZE_32x32, ui_palette, g_tile, 0, 128, OBJPROP_FIXED_POS);
	AnimationData *gear_anim = animdata_create(g_tile, ANIM_OFFSET_32x32, 3, 0);
	gameobj_set_anim_data(gear, gear_anim, 0);
	//gameobj_set_anim_info(gear, 3, ANIM_OFFSET_32x32, 0, false);
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
			hearts[i]->spr_tile_id = h_tile + 2;
		else
			hearts[i]->spr_tile_id = h_tile;
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
		int f_offset = action_counter[2]->spr_tile_id - a_tile;
		f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
		action_counter[2]->spr_tile_id = a_tile + f_offset;
		
		//if rolling up to a 0 or down to a 9
		if(((count_rolling > 0) && (displayed_action_count % 10 == 9)) || ((count_rolling < 0) && (displayed_action_count % 10 == 0)))
		{
			// update tens digit as well
			f_offset = action_counter[1]->spr_tile_id - a_tile;
			f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
			action_counter[1]->spr_tile_id = a_tile + f_offset;

			//repeat process for hundreds digit
			if(((count_rolling > 0) && (displayed_action_count % 100 == 99)) || ((count_rolling < 0) && (displayed_action_count % 100 == 0)))
			{
				f_offset = action_counter[0]->spr_tile_id - a_tile;
				f_offset = ((10 * DIGIT_ANIM_LENGTH) + (f_offset + count_rolling)) % (10 * DIGIT_ANIM_LENGTH);
				action_counter[0]->spr_tile_id = a_tile + f_offset;
			}
		}

		c_frame++;
		if(c_frame == DIGIT_ANIM_LENGTH)
		{
			c_frame = 0;
			displayed_action_count += count_rolling;
			if(displayed_action_count == true_action_count)
			{
				input_unlock_ui();
				count_rolling = 0;
				set_action_count_immediate(true_action_count);		//should be redundant if everything works properly, but just in case
			}
		}
	}

}




void reset_action_count()
{
	true_action_count = 0;
	displayed_action_count = 0;
	for(int i = 0; i < ACTION_COUNTER_DIGITS; i++)
	{
		action_counter[i]->spr_tile_id = a_tile;
		gameobj_update_attr(action_counter[i]);
	}
	set_action_count_immediate(0);
}

void set_action_count_immediate(int count)
{
	true_action_count = count;
	displayed_action_count = count;
	// hundreds digit
	action_counter[0]->spr_tile_id = a_tile + (DIGIT_ANIM_LENGTH * ((count / 100) % 10));
	gameobj_update_attr(action_counter[0]);
	// tens digit
	action_counter[1]->spr_tile_id = a_tile + (DIGIT_ANIM_LENGTH * ((count / 10) % 10));
	gameobj_update_attr(action_counter[1]);
	// ones digit
	action_counter[2]->spr_tile_id = a_tile + (DIGIT_ANIM_LENGTH * (count % 10));
	gameobj_update_attr(action_counter[2]);
}

void set_action_count(int count)
{
	true_action_count = count;
	if(true_action_count < displayed_action_count)
	{
		count_rolling = -1;
		c_frame = 0;
		ui_animate_gear_backward();
		input_lock_ui();
	}
	if(true_action_count > displayed_action_count)
	{
		count_rolling = 1;
		c_frame = 0;
		ui_animate_gear_forward();
		input_lock_ui();
	}
}

void increment_action_counter()
{
	displayed_action_count++;
	count_rolling = 1;
	c_frame = 0;
	ui_animate_gear_forward();
}

void decrement_action_counter()
{
	displayed_action_count--;
	count_rolling = -1;
	c_frame = 0;
	ui_animate_gear_backward();
}

void ui_animate_gear_forward()
{
	anim_play_forward(&gear->anim);
}

void ui_animate_gear_backward()
{
	anim_play_reversed(&gear->anim);
}