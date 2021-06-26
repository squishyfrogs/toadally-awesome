#include <tonc.h>
#include "game.h"
#include "vector2.h"
#include "direction.h"
#include "input.h"
#include "gameobj.h"
#include "objhistory.h"
#include "playerobj.h"
#include "frogtongue.h"
#include "map.h"


#define PLAYER_START_X			5		// player starting location (temp)
#define PLAYER_START_Y			5		// player starting location (temp)

const int hop_arc[16] = {0, 2, 4, 5, 6, 7, 7, 8, 8, 7, 7, 6, 5, 4, 2, 0};

// main.c
extern void action_update();
extern void finalize_turn();
extern void set_turn_active();
// camera.c
extern void camera_set_target(GameObj *target);
extern void set_camera_pos(int target_x, int target_y);
// effects.c
extern void create_effect_at_position(int tile_x, int tile_y);
// gameobj.c
extern void gameobj_push_changes(GameObj *obj);
// objinteract.c
extern void push_gameobj(GameObj *obj, int push_dir);

void playerobj_init();
void playerobj_update();

void move_playerobj(int input_x, int input_y);
void playerobj_update_movement();
void player_set_tile_by_id(int tile_id);

void push_gameobj(GameObj *obj, int push_dir);		// push a game object


static GameObj *player_obj;

static int p_palette;			// index of player palette in memory
static int p_tile_start;		// index of first tile of player sheet in memory

static Vector2 start_tile;		// start tile (for movement)
static Vector2 end_tile;		// end tile (for movement)
static Vector2 offset;			// pixel offset within one tile
static Vector2 mov;				// x and y speed+direction of current movement
static int hop_offset;			// number of pixels to shove sprite vertically to simulate hopping


GameObj *get_player_obj()
{
	return player_obj;
}

// PlayerObj init function
void playerobj_init()
{

	p_palette = mem_load_palette(playerPal);
	p_tile_start = mem_load_tiles(playerTiles, playerTilesLen);
	player_obj = gameobj_init_full(
		LAYER_GAMEOBJ, 
		ATTR0_SQUARE, 
		ATTR1_SIZE_16x16, 
		p_palette, 
		p_tile_start, 
		PLAYER_START_X, PLAYER_START_Y,
		OBJPROP_SOLID
		);
	AnimationData *player_anim = animdata_create(p_tile_start, ANIM_OFFSET_16x16, 2, PLAYER_FACING_OFFSET);
	gameobj_set_anim_data(player_obj, player_anim, ANIM_FLAG_LOOPING);
	//player_obj->anim = anim_create(p_tile_start, ANIM_OFFSET_16x16, 2, PLAYER_FACING_OFFSET, ANIM_FLAG_LOOPING);
	//player_update_current_tile();
	gameobj_update_current_tile(player_obj);
	//push_obj = NULL;
	camera_set_target(player_obj);
	gameobj_play_anim(player_obj);

	// initialize tongue 
	tongue_init(player_obj);
}

// main PlayerObj update
void playerobj_update()
{
	if(!input_locked())
	{
		if(key_hit(KEY_A))
			playerobj_action_primary();
		else if(key_hit(KEY_B))
			playerobj_action_secondary();
		else
			move_playerobj(key_tri_horz(), key_tri_vert());
	}
	
	if(gameobj_is_moving(player_obj))
		playerobj_update_movement();
	
	tongue_update();
}


// apply dpad inputs to the player and attempt to move them
void move_playerobj(int input_x, int input_y)
{
	if(gameobj_is_moving(player_obj))
		return;
	if(input_x == 0 && input_y == 0)
		return;

	//zero out y movement if diagonals are banned (clunky, replace with smart movement later maybe)
	if(!ALLOW_DIAGONAL && input_x != 0)
		input_y = 0;
	

	// update player facing direction
	if(!check_tongue_out())
	{
		if(input_x > 0)
			gameobj_set_facing(player_obj, DIRECTION_EAST);
		else if(input_x < 0)
			gameobj_set_facing(player_obj, DIRECTION_WEST);
		else if(input_y > 0)
			gameobj_set_facing(player_obj, DIRECTION_SOUTH);
		else if(input_y < 0)
			gameobj_set_facing(player_obj, DIRECTION_NORTH);
	}
	
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
			if(check_tile_free(end_tile.x + input_x, end_tile.y + input_y))
			{
				// if tile beyond push obj is free, valid to push object
				push_gameobj(contents, ints_to_dir(input_x, input_y));
				//gameobj_set_moving(contents, true, ints_to_dir(input_x, input_y));
				//create_effect_at_position(end_tile.x, end_tile.y);
				input_lock_player();
				set_turn_active();
			}
			// whether we can push the obj or not, end movement at this point
			// TODO: play bonk sound
			return;
		}
		// if immovable and solid
		else if(contents->obj_properties & OBJPROP_SOLID)
		{
			// TODO: play bonk sound
			return;
		}
	}

	// reset offsets (should already be 0 but just in case)
	offset.x = 0;
	offset.y = 0;
	hop_offset = 0;

	// set mov values
	mov.x = input_x * PLAYER_MOVE_SPEED;
	mov.y = input_y * PLAYER_MOVE_SPEED;

	// lock inputs
	input_lock_player();
	// set the turn active
	set_turn_active();
	// mark player as moving
	gameobj_set_moving(player_obj, true, ints_to_dir(input_x, input_y));
	// player_moving = true;
	// perform an action update
	action_update();
}


// called every frame to keep player in motion
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

	// check if we moved a full tile, and if so, stop movement 
	if((offset.x >= GAME_TILE_SIZE) || (offset.x <= -GAME_TILE_SIZE))
		mov.x = 0;
	if((offset.y >= GAME_TILE_SIZE) || (offset.y <= -GAME_TILE_SIZE))
		mov.y = 0;
	
	

	gameobj_set_pixel_pos(player_obj, offset.x, offset.y - hop_offset);		// subtract hop_offset to show verticality

	Vector2 v = gameobj_get_pixel_pos(player_obj);
	set_camera_pos(v.x, v.y + hop_offset);

	
	
	if(mov.x == 0 && mov.y == 0)
	{
		//player_update_current_tile();
		gameobj_update_current_tile(player_obj);
		gameobj_set_moving(player_obj,false, 0);
	}

}




void playerobj_action_primary()
{
	tongue_extend();
}

// perform the B press action
void playerobj_action_secondary()
{
	tongue_retract();
}




// checks whether or not a given GameObj is the PlayerObj
bool gameobj_is_player(GameObj *obj)
{
	return (obj == player_obj && obj != NULL);
}