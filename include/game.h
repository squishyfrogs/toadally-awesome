#ifndef GAME_H
#define GAME_H

#include <tonc.h>
#include "vector2.h"

typedef enum GameState_T{
	GS_DEBUG,
	GS_STARTUP,
	GS_LOGO,
	GS_TITLE,
	GS_LEVEL_SELECT,
	GS_MAIN_GAME,
	GS_CREDITS
} GameState;


#define ANIM_SPEED		5			// 60/speed = anim fps 
#define UI_ANIM_SPEED	3			// 60/speed = anim fps 
#define GAME_TILE_SIZE	16			// tile size in pixels




void set_turn_active();
void set_turn_inactive();
bool check_turn_active();
void action_update();								// update that occurs when the player takes an action
void finalize_turn();								// update that occurs after all pieces have settled


void set_game_state(GameState state);
GameState get_game_state();

void set_game_paused(bool paused);
bool check_game_paused();

void set_world_offset(int off_x, int off_y);
Vector2 get_world_offset();
void update_world_pos();
void REGBGOFS_reset_all();


#endif //GAME_H