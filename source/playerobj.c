#include <tonc.h>
#include "game.h"
#include "vector2.h"
#include "direction.h"
#include "input.h"
#include "gameobj.h"
#include "playerobj.h"
#include "objhistory.h"
#include "map.h"


#define PLAYER_START_X			5		// player starting location (temp)
#define PLAYER_START_Y			5		// player starting location (temp)

const int hop_arc[16] = {0, 2, 4, 5, 6, 7, 7, 8, 8, 7, 7, 6, 5, 4, 2, 0};

// main.c
extern void action_update();
extern void finalize_turn();
// camera.c
extern void camera_set_target(GameObj *target);

// gameobj.c
extern void gameobj_push_changes(GameObj *obj);



void playerobj_init();
void playerobj_update();

void move_playerobj(int input_x, int input_y);
void playerobj_update_movement();
void player_update_current_tile();
void player_set_tile_by_id(int tile_id);

void set_push_obj(GameObj *obj);		// set current push obj
void free_push_obj();					// free current push obj

static GameObj *player_obj;
//static ObjHistory *playerobj_history;

static int p_palette;			// index of player palette in memory
static int p_tile_start;		// index of first tile of player sheet in memory

//static Vector2 current_tile;	// map tile the player is currently on (updated at rest)

static bool player_moving;		// is the player currently moving? 

static Vector2 start_tile;		// start tile (for movement)
static Vector2 end_tile;		// end tile (for movement)
static Vector2 offset;			// pixel offset within one tile
static Vector2 mov;				// x and y speed+direction of current movement
static int hop_offset;			// number of pixels to shove sprite vertically to simulate hopping

GameObj *push_obj;				// object the player is currently pushing

GameObj *get_player_obj()
{
	return player_obj;
}

void playerobj_init()
{

	p_palette = mem_load_palette(playerPal);
	p_tile_start = mem_load_tiles(playerTiles, playerTilesLen);
	player_obj = init_gameobj_full(
		LAYER_GAMEOBJ, 
		ATTR0_SQUARE, 
		ATTR1_SIZE_16x16, 
		p_palette, 
		p_tile_start, 
		PLAYER_START_X, PLAYER_START_Y,
		OBJPROP_SOLID
		);
	player_obj->anim = anim_create(p_tile_start, ANIM_OFFSET_16x16, 0, PLAYER_FACING_OFFSET, ANIM_FLAG_LOOPING);
	player_update_current_tile();
	push_obj = NULL;
	camera_set_target(player_obj);
	
}

void playerobj_update()
{
	if(!input_locked())
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

	//zero out y movement if diagonals are banned (clunky, replace with smart movement later maybe)
	if(!ALLOW_DIAGONAL && input_x != 0)
		input_y = 0;
	
	// update player facing direction
	if(input_x > 0)
		gameobj_set_facing(player_obj, DIRECTION_EAST);
	else if(input_x < 0)
		gameobj_set_facing(player_obj, DIRECTION_WEST);
	else if(input_y > 0)
		gameobj_set_facing(player_obj, DIRECTION_SOUTH);
	else if(input_y < 0)
		gameobj_set_facing(player_obj, DIRECTION_NORTH);
	
	// update tile start and tile end 
	start_tile.x = player_obj->tile_pos.x;
	start_tile.y = player_obj->tile_pos.y;
	end_tile.x = start_tile.x + input_x;
	end_tile.y = start_tile.y + input_y;

	// constrain player movement to map boundaries
	if(end_tile.x < 0)
	{
		end_tile.x = 0;
		input_x = 0;
		return;
	}
	if(end_tile.x >= MAP_SIZE_X)
	{
		end_tile.x = MAP_SIZE_X-1;
		input_x = 0;
		return;
	}
	if(end_tile.y < 0)
	{
		end_tile.y = 0;
		input_y = 0;
		return;
	}
	if(end_tile.y >= MAP_SIZE_Y)
	{
		end_tile.y = MAP_SIZE_Y-1;
		input_y = 0;
		return;
	}

	// check that dest tile is a valid height to jump to
	ushort start_height = get_tile_col_info(start_tile.x,start_tile.y);
	ushort dest_height = get_tile_col_info(end_tile.x,end_tile.y);
	if((dest_height == 0) || (start_height - dest_height > 2) || (dest_height - start_height > 2))
		return;

	// check that dest tile is empty
	GameObj *contents = get_tile_contents(end_tile.x, end_tile.y);
	if(contents != NULL)
	{
		// attempt to move object
		if(contents->obj_properties & OBJPROP_MOVABLE)
		{
			// check tile past obj
			// TODO: Constrain this to map bounds
			GameObj *far_tile_contents = get_tile_contents(end_tile.x + input_x, end_tile.y + input_y);
			if(far_tile_contents == NULL || !(far_tile_contents->obj_properties & OBJPROP_SOLID))
			{
				// valid to push object
				//place_obj_in_tile(contents, end_tile.x + input_x, end_tile.y + input_y);
				remove_tile_contents(contents, end_tile.x, end_tile.y);
				set_tile_contents(contents, end_tile.x + input_x, end_tile.y + input_y);
				set_push_obj(contents);
			}
		}
		// if immovable and solid
		else if(contents->obj_properties & OBJPROP_SOLID)
			return;
	}

	// reset offsets (should already be 0 but just in case)
	offset.x = 0;
	offset.y = 0;
	hop_offset = 0;

	// set mov values
	mov.x = input_x * PLAYER_MOVE_SPEED;
	mov.y = input_y * PLAYER_MOVE_SPEED;

	// mark player as moving
	player_moving = true;
	// perform an action update
	action_update();
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
	
	


	gameobj_set_pixel_pos(player_obj, offset.x, offset.y - hop_offset);		// subtract hop_offset to show verticality
	if(push_obj != NULL)
	{
		gameobj_set_pixel_pos(push_obj, offset.x, offset.y);
	}


	
	if(mov.x == 0 && mov.y == 0)
	{
		player_update_current_tile();
		player_moving = false;
		if(push_obj)
		{
			gameobj_update_current_tile(push_obj);
			free_push_obj();
		}
		
		// finalize the turn when all objects come to rest
		finalize_turn();
	}

}

void player_update_current_tile()
{
	// free players old tile
	remove_tile_contents(player_obj, player_obj->tile_pos.x, player_obj->tile_pos.y);

	if((player_obj->pixel_pos.x >= GAME_TILE_SIZE) || (player_obj->pixel_pos.x <= -GAME_TILE_SIZE))
	{
		player_obj->tile_pos.x += (player_obj->pixel_pos.x / GAME_TILE_SIZE);
		player_obj->pixel_pos.x %= GAME_TILE_SIZE;
	}
	if((player_obj->pixel_pos.y >= GAME_TILE_SIZE) || (player_obj->pixel_pos.y <= -GAME_TILE_SIZE))
	{
		player_obj->tile_pos.y += (player_obj->pixel_pos.y / GAME_TILE_SIZE);
		player_obj->pixel_pos.y %= GAME_TILE_SIZE;
	}
	
	
	// claim new tile
	set_tile_contents(player_obj, player_obj->tile_pos.x, player_obj->tile_pos.y);
}


void set_push_obj(GameObj *obj)
{
	push_obj = obj;
}

void free_push_obj()
{
	push_obj = NULL;
}