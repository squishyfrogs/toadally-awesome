#include <tonc.h>
#include "game.h"
#include "gameobj.h"
#include "map.h"
#include "vector2.h"
#include "sprites/player.h"


#define MOVE_SPEED 1			// move speed in pixels 

void playerobj_init();
void playerobj_update();

void move_playerobj(int input_x, int input_y);
void playerobj_update_movement();
void update_current_tile();


static GameObj *player_obj;

#define PLAYER_START_X			80		// player starting location
#define PLAYER_START_Y			80		// player starting location
#define PLAYER_TILE_OFFSET		4		// TILE offset for animations (how many tiles is one frame?)
#define PLAYER_FACING_OFFSET	8		// FRAME offset for each direction (multiply by tile offset for full offset)


static int p_palette;			// index of player palette in memory
static int p_tile_start;		// index of first tile of player sheet in memory
static Vector2 current_tile;	// tile the player is currently on (updated at rest)

static bool player_moving;		// is the player currently moving? 
//static int start_x, start_y;	// position player started moving from	REPLACE	
//static int end_x, end_y;		// destination position					REPLACE


static Vector2 start_tile;		// start tile (for movement)
static Vector2 end_tile;		// end tile (for movement)
static Vector2 offset;			// pixel offset within one tile
static Vector2 mov;				// x and y speed+direction of current movement
static int hop_offset;			// number of pixels to shove sprite vertically to simulate hopping
const int hop_arc[16] = {0, 2, 4, 5, 6, 7, 7, 8, 8, 7, 7, 6, 5, 4, 2, 0};

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

	update_current_tile();
	
}

void playerobj_update()
{
	move_playerobj(key_tri_horz(), key_tri_vert());
	
	if(player_moving)
		playerobj_update_movement();
}

// apply dpad inputs to the player and attempt to move them
void move_playerobj(int input_x, int input_y)
{
	if(player_moving)
		return;
	if(input_x == 0 && input_y == 0)
		return;
	
	// update tile start and tile end 
	start_tile.x = current_tile.x;
	start_tile.y = current_tile.y;
	end_tile.x = start_tile.x + input_x;
	end_tile.y = start_tile.y + input_y;

	// check that dest tile is a valid height to jump to
	ushort start_height = get_tile_col_info(start_tile.x,start_tile.y);
	ushort dest_height = get_tile_col_info(end_tile.x,end_tile.y);
	if((dest_height == 0) || (start_height - dest_height > 2) || (dest_height - start_height > 2))
		return;

	// reset offsets (should already be 0 but just in case)
	offset.x = 0;
	offset.y = 0;
	hop_offset = 0;

	// set mov values
	mov.x = input_x * MOVE_SPEED;
	mov.y = input_y * MOVE_SPEED;

	// mark player as moving
	player_moving = true;
}

void playerobj_update_movement()
{
	offset.x += mov.x;
	offset.y += mov.y;

	//update hop offset
	if(mov.x != 0)
	{
		hop_offset = hop_arc[((GAME_TILE_SIZE + offset.x) % GAME_TILE_SIZE)];
	}
	else
	{
		hop_offset = hop_arc[((GAME_TILE_SIZE + offset.y) % GAME_TILE_SIZE)];
	}

	// check if we moved a full tile
	if((offset.x >= GAME_TILE_SIZE) || (offset.x <= -GAME_TILE_SIZE))
		mov.x = 0;
	if((offset.y >= GAME_TILE_SIZE) || (offset.y <= -GAME_TILE_SIZE))
		mov.y = 0;
	
	


	int x = start_tile.x*GAME_TILE_SIZE + offset.x;
	int y = start_tile.y*GAME_TILE_SIZE + offset.y - hop_offset;	// subtract hop_offset to go up
	gameobj_set_pos(player_obj, x, y);

	if(mov.x == 0 && mov.y == 0)
	{
		update_current_tile();
		player_moving = false;
	}

}

void update_current_tile()
{
	current_tile.x = player_obj->pos_x/GAME_TILE_SIZE;
	current_tile.y = player_obj->pos_y/GAME_TILE_SIZE;
}