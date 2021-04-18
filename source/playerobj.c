#include <tonc.h>
#include "gameobj.h"
#include "sprites/player.h"

#define GAME_TILE_SIZE 16		// tile size in pixels
#define MOVE_SPEED 1			// move speed in pixels 

void playerobj_init();
void playerobj_update();

void move_playerobj(int input_x, int input_y);
void playerobj_update_movement();


static GameObj *player_obj;

#define PLAYER_START_X			80		// player starting location
#define PLAYER_START_Y			80		// player starting location
#define PLAYER_TILE_OFFSET		4		// TILE offset for animations (how many tiles is one frame?)
#define PLAYER_FACING_OFFSET	8		// FRAME offset for each direction (multiply by tile offset for full offset)


static int p_palette;			// index of player palette in memory
static int p_tile_start;		// index of first tile of player sheet in memory
static bool player_moving;		// is the player currently moving? 
static int mov_x, mov_y;		// x and y components of current movement
static int start_x, start_y;	// position player started moving from
static int end_x, end_y;		// destination position

void playerobj_init()
{

	player_obj = init_gameobj();
	p_palette = mem_load_palette(playerPal);
	p_tile_start = mem_load_tiles(playerTiles, playerTilesLen);
	gameobj_update_attr_full(
		player_obj, 
		ATTR0_SQUARE, 
		ATTR1_SIZE_16x16, 
		p_palette, 
		p_tile_start, 
		PLAYER_START_X, PLAYER_START_Y, 
		false);

	
	
}

void playerobj_update()
{
	move_playerobj(key_tri_horz(), key_tri_vert());
	
	playerobj_update_movement();
}

// apply dpad inputs to the player and attempt to move them
void move_playerobj(int input_x, int input_y)
{
	if(player_moving)
		return;
	if(input_x == 0 && input_y == 0)
		return;
	//input_x = clamp(input_x, -1, 1);
	//input_y = clamp(input_y, -1, 1);

	start_x = player_obj->pos_x;
	start_y = player_obj->pos_y;

	end_x = start_x + input_x * GAME_TILE_SIZE;
	end_y = start_y + input_y * GAME_TILE_SIZE;

	mov_x = input_x * MOVE_SPEED;
	mov_y = input_y * MOVE_SPEED;

	player_moving = true;
}

void playerobj_update_movement()
{
	//gameobj_change_pos(player_obj, mov_x, mov_y);
	int x = player_obj->pos_x;
	int y = player_obj->pos_y;
	if((x - end_x) * ((x+mov_x) - end_x) <= 0)		//check if we cross end_x
	{
		x = end_x;
		mov_x = 0;
	}
	if((y - end_y) * ((y+mov_y) - end_y) <= 0)		//check if we cross end_y
	{
		y = end_y;
		mov_y = 0;
	}	
		
	x += mov_x;
	y += mov_y;

	if(mov_x == 0 && mov_y == 0)
		player_moving = false;
	
	gameobj_set_pos(player_obj, x, y);
}
