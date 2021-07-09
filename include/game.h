#ifndef GAME_H
#define GAME_H

#include <tonc.h>
#include "vector2.h"

typedef enum GameState_T{
	GS_DEBUG			= 0,
	GS_STARTUP			= 1,
	GS_LOGO				= 2,
	GS_TITLE			= 3,
	GS_LEVEL_SELECT		= 4,
	GS_MAIN_GAME		= 5,
	GS_CREDITS			= 6
} GameState;


#define ANIM_SPEED		5			// 60/speed = anim fps 
#define UI_ANIM_SPEED	3			// 60/speed = anim fps 
#define GAME_TILE_SIZE	16			// tile size in pixels




void set_turn_active();
void set_turn_inactive();
bool check_turn_active();
void action_update();								// update that occurs when the player takes an action
void finalize_turn();								// update that occurs after all pieces have settled


GameState get_game_state();
void go_to_game_state(GameState gs);				// calls for a change of gamestate, which will be executed at the END of the current frame

void set_game_paused(bool paused);
bool check_game_paused();

void set_world_offset(int off_x, int off_y);
Vector2 get_world_offset();
void update_world_pos();
void REGBGOFS_reset_all();


#endif //GAME_H