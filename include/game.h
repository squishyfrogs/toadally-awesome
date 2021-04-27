#ifndef GAME_H
#define GAME_H

enum GameState{
	GS_DEBUG			= 0,
	GS_TITLE			= 1,
	GS_LEVEL_SELECT		= 2,
	GS_MAIN_GAME		= 3,
};


#define ANIM_SPEED		5			// 60/speed =  fps 
#define UI_ANIM_SPEED	3			// 60/speed =  fps 
#define GAME_TILE_SIZE	16			// tile size in pixels

#define CAMERA_BOUNDS_HORIZONTAL	32
#define CAMERA_BOUNDS_VERTICAL		32


#endif //GAME_H