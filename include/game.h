#ifndef GAME_H
#define GAME_H



typedef enum GameState_T{
	GS_DEBUG,
	GS_STARTUP,
	GS_LOGO,
	GS_TITLE,
	GS_MAP_SELECT,
	GS_MAIN_GAME
} GameState;


#define ANIM_SPEED		5			// 60/speed = anim fps 
#define UI_ANIM_SPEED	3			// 60/speed = anim fps 
#define GAME_TILE_SIZE	16			// tile size in pixels


void set_game_state(GameState state);
GameState get_game_state();

#endif //GAME_H