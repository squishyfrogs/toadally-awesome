#include <string.h>
#include <tonc.h>
#include <maxmod.h>

#include "game.h"
#include "debug.h"
#include "gamedata.h"
#include "regmem.h"
#include "audio.h"
#include "layers.h"
#include "input.h"
#include "screens.h"
#include "palettes.h"


extern void audio_init();				// audio.c
extern void animdata_init_all();		// animation.c
extern void playerobj_init();			// playerobj.c
extern void map_init();					// map.c
// game.c
extern void set_game_state(GameState state);
extern void game_update_main();
extern void update_world_pos();
// text.c
extern void textsys_init();
// level.c
extern void load_level_info();
extern void set_level_data(int level_id);
// levelselect.c
extern void level_select_update();
// effects.c
extern void effects_init();
extern void effects_anim_update();
// playerobj.c
extern void playerobj_update();
// ui.c
extern void ui_init();
extern void ui_update();
extern void ui_update_anim();
// gameobj.c
extern void gameobj_init_all();
extern void gameobj_update_all();
extern void gameobj_update_anim_all();
extern void gameobj_push_all_updates();
extern bool gameobj_all_at_rest();
// objinteract.c
extern void objint_init();
// colors.c
extern void color_cycle_init();
extern void color_cycle_update();
// pausemenu.c
extern void pausemenu_init();
extern void pausemenu_update();


void main_loop();

// init functions
void global_init();
void main_game_init();
void main_game_end();
void global_soft_reset();

void go_to_logo();									// go to the logo screen (the first scene upon game startup)
void go_to_title();									// go to the title screen
void go_to_level_select();
void go_to_main_game();
static void (*state_change)();						// state change function to execute at the end of each frame (must point to one of the above functions, or be NULL)


void main_game_start();								// after initializing everything, this is called to set the game in motion, then we enter the game loop

// update functions
void title_update();								// update title screen
void graphics_update();								// update graphics and animation
void game_update();									// update gameplay elements







////////////
/// Main ///
////////////

// The Big One
int main(void)
{
	global_init();
	
	if(DEBUG_SKIP_INTROS)
	{
		main_game_init();
		set_level_data(0);
		go_to_main_game();
	}
	else
		go_to_logo();

	
	//main_game_start();

	main_loop();


	return 0;
}

// Where all the magic happens
void main_loop()
{
	while (1) 
	{
		//vid_vsync();		//resource hog
		mmFrame();			// audio update (must call every frame)
		VBlankIntrWait();	//slower but saves power
		key_poll();


		//if(key_is_down(KEY_SELECT))
		if(key_is_down(KEY_RESET) == KEY_RESET)
		{	
			if(get_game_state() != GS_LOGO)
				break;		// exit the loop 
		}

		graphics_update();


		switch(get_game_state())
		{
			case GS_LOGO:
			case GS_TITLE:
				title_update();
				break;
			case GS_LEVEL_SELECT:
				level_select_update();
				break;
			case GS_MAIN_GAME:
			default:
				game_update();
				break;
		}

		if(state_change != NULL)
		{
			state_change();
		}
		
	}
	// if out here for whatever reason, reboot the game
	global_soft_reset();
}



//////////////////////////////////
/// Primary Game Init Function ///
//////////////////////////////////

//all initialization and setup goes in here
void global_init()
{	
	//set_game_state(GS_DEBUG);
	set_game_state(GS_STARTUP);
	gamedata_init();

	// GBA interrupt setup
	irq_init(NULL);
	irq_add(II_VBLANK, mmVBlank);
	irq_enable(II_VBLANK);
	//irq_init(NULL);
	//irq_add(II_VBLANK, NULL);
	//irq_enable(II_VBLANK);
	audio_init();
	textsys_init();
	color_cycle_init();
	gameobj_init_all();
}



void main_game_init()
{
	//reg_set_main();
	// game setup
	animdata_init_all();
	objint_init();
	//gameobj_init_all();

	map_init();
	playerobj_init();
	ui_init();
	effects_init();
	pausemenu_init();
	
	textsys_init();		// call in global init should be sufficient, but it isn't and i'm out of time to investigate why lol
	

	// temp
	//init_objs_temp();
	//test_init_tte_se4();
	//test_run_tte_se4();
	//
}



void global_soft_reset()
{
	audio_stop_all();
	main_game_end();
	input_unlock_override_all();
	REG_BLDY = 0;
	REG_DISPCNT = 0;
	SoftReset();
}


//////////////////
/// Game States ///
//////////////////

// calls for a change of gamestate, which will be executed at the END of the current frame
void go_to_game_state(GameState gs)
{
	switch(gs)
	{
		case GS_LOGO:
			state_change = go_to_logo;
			break;
		case GS_TITLE:
			state_change = go_to_title;
			break;
		case GS_LEVEL_SELECT:
			state_change = go_to_level_select;
			break;
		case GS_MAIN_GAME:
			state_change = go_to_main_game;
			break;
		default:
			state_change = NULL;
			break;
	}
}



void go_to_logo()
{
	state_change = NULL;
	unload_current_screen();
	set_game_state(GS_LOGO);
	input_lock(INPLCK_SYS);
	logo_load();
	reg_set_title();
	logo_display();
}


void go_to_title()
{
	state_change = NULL;
	unload_current_screen();
	set_game_state(GS_TITLE);
	input_lock(INPLCK_SYS);
	title_load();
	audio_play_track(MOD_LEV_SEL_2);
	reg_set_title();
	title_display();
}


void go_to_level_select()
{
	state_change = NULL;
	unload_current_screen();
	set_game_state(GS_LEVEL_SELECT);
	lev_sel_load();
	reg_set_lev_sel();
	lev_sel_display();
}

void go_to_main_game()
{
	state_change = NULL;
	unload_current_screen();
	set_game_state(GS_MAIN_GAME);
	main_game_init();
	load_main_game_palettes();
	main_game_start();
}





/////////////////////////////
/// Game Update Functions ///
/////////////////////////////


// update title screen
void title_update()
{
	if(key_hit(KEY_NONDIR))
	{
		switch(get_game_state())
		{
			case GS_LOGO:
				go_to_title();
				break;
			case GS_TITLE:
				go_to_level_select();
				break;
			case GS_LEVEL_SELECT:
				go_to_main_game();
				break;
			default:
				break;
		}
		
	}
}

extern bool playerhealth_is_dead();

// update gameplay elements
void game_update()
{
	
	// gameplay update functions go out here
	playerobj_update();				// update player first
	gameobj_update_all();			// update gameobj movement
	game_update_main();				
	update_world_pos();				// push the map around
	pausemenu_update();
	ui_update();

	// finalize the turn when all objects come to rest
	if(check_turn_active() && gameobj_all_at_rest() && !playerhealth_is_dead())
	{
		finalize_turn();
		input_unlock(INPLCK_SYS);
	}

	// update gameobj attrs based on gameplay changes
	gameobj_push_all_updates();

}


// update graphics and animation
void graphics_update()
{
	static uint32_t anim_sync;
	static uint32_t ui_anim_sync;
	

	anim_sync++;
	if(anim_sync > ANIM_SPEED)
	{
		// animation update functions go in here
		gameobj_update_anim_all();
		effects_anim_update();
		anim_sync %= ANIM_SPEED;
	}
	ui_anim_sync++;
	if(ui_anim_sync > UI_ANIM_SPEED)
	{
		color_cycle_update();
		//ui looked weird at the slower anim speed so it runs on its own cycle
		ui_update_anim();
		ui_anim_sync %= UI_ANIM_SPEED;
	}
}















/////////////////////////////
/// Testing & Placeholder ///
/////////////////////////////




// initialize game timers (currently unused?)
//void timer_init()
//{
	// Overflow every ~1 second:
	// 0x4000 ticks @ FREQ_1024

	// REG_TM2D = -0x4000;				// 111 ticks to overflow - should produce 60hz cycle?
	// REG_TM2CNT = TM_FREQ_1024;		// use 1024 cycle timer
	// REG_TM2CNT = TM_ENABLE;
	// REG_TM3CNT= TM_ENABLE | TM_CASCADE;		// tm1 cascades into tm0
//}



