#include <tonc.h>
#include "game.h"
#include "map.h"

#define SCREEN_HALF_WIDTH (SCREEN_WIDTH/2)		// half screen width
#define SCREEN_HALF_HEIGHT (SCREEN_HEIGHT/2)	// half screen height
#define CAMERA_BOUNDS_HORIZONTAL	32
#define CAMERA_BOUNDS_VERTICAL		32
#define PLAYER_SIZE					16			// player size in pixels

void update_camera_pos(int target_x, int target_y);
extern void set_world_offset(int off_x, int off_y);


extern int world_offset_x;
extern int world_offset_y;


void update_camera_pos(int target_x, int target_y)
{
	int cam_x = world_offset_x + SCREEN_HALF_WIDTH;
	int cam_y = world_offset_y + SCREEN_HALF_HEIGHT;
	int cam_bound_h = (SCREEN_HALF_WIDTH) - CAMERA_BOUNDS_HORIZONTAL;
	int cam_bound_v = (SCREEN_HALF_HEIGHT) - CAMERA_BOUNDS_VERTICAL;

	//try to constrain player within camera bounds
	if(cam_x < target_x - cam_bound_h + PLAYER_SIZE)
		cam_x = target_x - cam_bound_h + PLAYER_SIZE;
	if(cam_x > target_x + cam_bound_h)
		cam_x = target_x + cam_bound_h;
	if(cam_y < target_y - cam_bound_v + PLAYER_SIZE)
		cam_y = target_y - cam_bound_v + PLAYER_SIZE;
	if(cam_y > target_y + cam_bound_v)
		cam_y = target_y + cam_bound_v;
	
	//maintain map bounds above all else
	if(cam_x < SCREEN_HALF_WIDTH)
		cam_x = SCREEN_HALF_WIDTH;
	if(cam_x > (MAP_SIZE_X * GAME_TILE_SIZE) - SCREEN_HALF_WIDTH)
		cam_x = (MAP_SIZE_X * GAME_TILE_SIZE) - SCREEN_HALF_WIDTH;

	if(cam_y < SCREEN_HALF_HEIGHT)
		cam_y = SCREEN_HALF_HEIGHT;
	if(cam_y > (MAP_SIZE_Y * GAME_TILE_SIZE) - SCREEN_HALF_HEIGHT)
		cam_y = (MAP_SIZE_Y * GAME_TILE_SIZE) - SCREEN_HALF_HEIGHT;

	
	set_world_offset(cam_x - SCREEN_HALF_WIDTH, cam_y - SCREEN_HALF_HEIGHT);
}