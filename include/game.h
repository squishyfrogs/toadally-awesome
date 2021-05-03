#ifndef GAME_H
#define GAME_H



typedef enum GameState_T{
	GS_DEBUG			= 0,
	GS_LOGO				= 1,
	GS_TITLE			= 2,
	GS_LEVEL_SELECT		= 3,
	GS_MAIN_GAME		= 4
} GameState;


#define ANIM_SPEED		5			// 60/speed =  fps 
#define UI_ANIM_SPEED	3			// 60/speed =  fps 
#define GAME_TILE_SIZE	16			// tile size in pixels



#endif //GAME_H