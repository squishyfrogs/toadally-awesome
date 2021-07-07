#include <tonc.h>
#include "debug.h"
#include "game.h"
#include "vector2.h"
#include "direction.h"
#include "input.h"
#include "gameobj.h"
#include "objhistory.h"
#include "playerobj.h"
#include "playerhealth.h"
#include "frogtongue.h"
#include "objinteract.h"
#include "map.h"
#include "audio.h"
#include "regmem.h"
#include "palettes.h"


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

void playerobj_init();
void player_anim_init();		// initialize anims
void player_anim_create(PlayerAnimID pid, int offset, int len);
void playerobj_update();

void playerobj_input_direction(int input_x, int input_y);
void playerobj_move(int move_x, int move_y);
void playerobj_update_movement();
void playerobj_update_facing(int x, int y);

int playerobj_current_hop_height();

static GameObj *player_obj;
static AnimationData *player_anims[PAI_COUNT];

//static int p_palette;			// index of player palette in memory
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

	//p_palette = mem_load_palette(spr_playerPal, PAL_ID_PLAYER);
	p_tile_start = mem_load_tiles(spr_playerTiles, spr_playerTilesLen);
	player_obj = gameobj_init_full(
		LAYER_GAMEOBJ, 
		ATTR0_SQUARE, 
		ATTR1_SIZE_16x16, 
		PAL_ID_PLAYER, 
		p_tile_start, 
		PLAYER_START_X, PLAYER_START_Y,
		OBJPROP_SOLID
		);
	register_obj_history(player_obj);
	player_anim_init();
	gameobj_update_current_tile(player_obj);
	camera_set_target(player_obj);
	playerobj_play_anim(PAI_IDLE);
	gameobj_play_anim(player_obj);

	// initialize tongue 
	tongue_init(player_obj);
	// initialize health
	playerhealth_init();
}

// initialize anims
void player_anim_init()
{
	player_anim_create(PAI_IDLE, 0, 2);
	player_anim_create(PAI_HOP, 1, 1);
	player_anim_create(PAI_TONGUE, 2, 1);
	player_anim_create(PAI_NOM, 1, 1);
}

void player_anim_create(PlayerAnimID pid, int offset, int len)
{
	offset *= ANIM_OFFSET_16x16;
	player_anims[pid] = animdata_create(p_tile_start + offset, ANIM_OFFSET_16x16, len, PLAYER_FACING_OFFSET);
}

// main PlayerObj update
void playerobj_update()
{
	if(!input_locked() && !gameobj_is_moving(player_obj) && !history_mode_active())
	{
		if(key_hit(KEY_A))
			playerobj_action_primary();
		else if(key_hit(KEY_B))
			playerobj_action_secondary();
		else
			playerobj_input_direction(key_tri_horz(), key_tri_vert());
	}
	
	if(gameobj_is_moving(player_obj))
		playerobj_update_movement();
	
	tongue_update();
}


// apply dpad inputs to the player and attempt to move them
void playerobj_input_direction(int input_x, int input_y)
{	
	// check for an object attached to the tongue 
	GameObj *obj_att = tongue_get_attached_object();
	// if one exists, disable inputs perpendicular to the facing dir
	if(obj_att != NULL)
	{
		switch(gameobj_get_facing(player_obj))
		{
			case DIRECTION_NORTH:
			case DIRECTION_SOUTH:
				input_x = 0;
				break;
			case DIRECTION_WEST:
			case DIRECTION_EAST:
				input_y = 0;
				break;
			default:
				break;
		}
	}


	// can't move if no inputs
	if(input_x == 0 && input_y == 0)
		return;

	//zero out y movement if diagonals are banned (clunky, replace with smart diagonal reading later maybe)
	if(!ALLOW_DIAGONAL && input_x != 0)
		input_y = 0;
	

	// update player facing direction if tongue is not extended
	if(!check_tongue_out())
	{
		playerobj_update_facing(input_x, input_y);
	}
	
	// TODO: add a ~3 frame buffer for the input to be held before committing to movement
	playerobj_move(input_x, input_y);
}

// move the player
void playerobj_move(int move_x, int move_y)
{	
	// update tile start and tile end 
	start_tile.x = player_obj->tile_pos.x;
	start_tile.y = player_obj->tile_pos.y;
	end_tile.x = start_tile.x + move_x;
	end_tile.y = start_tile.y + move_y;

	// constrain player movement to map boundaries
	if(end_tile.x < 0)
	{
		end_tile.x = 0;
		move_x = 0;
		return;
	}
	if(end_tile.x >= MAP_SIZE_X)
	{
		end_tile.x = MAP_SIZE_X-1;
		move_x = 0;
		return;
	}
	if(end_tile.y < 0)
	{
		end_tile.y = 0;
		move_y = 0;
		return;
	}
	if(end_tile.y >= MAP_SIZE_Y)
	{
		end_tile.y = MAP_SIZE_Y-1;
		move_y = 0;
		return;
	}

	// For now, variable tile height has been scrapped. Code left in place for if I bring it back someday
	// check that dest tile is a valid height to jump to
	//	ushort start_height = get_tile_properties(start_tile.x,start_tile.y);
	//	ushort dest_height = get_tile_properties(end_tile.x,end_tile.y);
	//	if((dest_height == 0) || (start_height - dest_height > 2) || (dest_height - start_height > 2))
	//	return;
	//ushort tile_props = get_tile_properties(end_tile.x, end_tile.y);
	if(!check_tile_free(end_tile.x, end_tile.y))
		return;

	// check that dest tile is empty
	GameObj *contents = get_tile_contents(end_tile.x, end_tile.y);
	if(contents != NULL)
	{
		// attempt to move object
		if(gameobj_check_properties(contents, OBJPROP_MOVABLE))
		{
			// check tile past obj
			if(check_tile_free(end_tile.x + move_x, end_tile.y + move_y))
			{
				// if tile beyond push obj is free, valid to push object
				objint_push_gameobj(contents, ints_to_dir(move_x, move_y));
				//gameobj_set_moving(contents, true, ints_to_dir(input_x, input_y));
				//create_effect_at_position(end_tile.x, end_tile.y);
				input_lock(INPLCK_PLAYER);
				set_turn_active();
			}
			// whether we can push the obj or not, end movement at this point
			// TODO: play bonk sound
			return;
		}
		// if immovable and solid
		else if(gameobj_check_properties(contents, OBJPROP_SOLID))
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
	mov.x = move_x * PLAYER_MOVE_SPEED;
	mov.y = move_y * PLAYER_MOVE_SPEED;

	// lock inputs
	input_lock(INPLCK_PLAYER);
	// set the turn active
	set_turn_active();
	// mark player as moving
	int mov_dir = ints_to_dir(move_x, move_y);
	gameobj_set_moving(player_obj, true, mov_dir);
	// play hop anim
	if(!check_tongue_out())
		playerobj_play_anim(PAI_HOP);
	// play hop sfx
	audio_play_sound(SFX_FROG_HOP);

	// check for an object attached to the tongue 
	GameObj *obj_att = tongue_get_attached_object();
	// move the attached tongue obj if applicable
	if(obj_att != NULL)
	{
		// if moving away from obj_att, pull it
		if(gameobj_get_facing(player_obj) != mov_dir)
			gameobj_set_moving(obj_att, true, mov_dir);
		// if moving toward obj_att, contract tongue
		else
			tongue_contract();
	}

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
	set_camera_pos(v.x, v.y + hop_offset);									// add hop_offset back in to keep camera smooth

	
	
	if(mov.x == 0 && mov.y == 0)
	{
		//player_update_current_tile();
		gameobj_update_current_tile(player_obj);
		gameobj_set_moving(player_obj,false, 0);
		if(!check_tongue_out())
			playerobj_play_anim(PAI_IDLE);
	}

}

// update direction player is facing
void playerobj_update_facing(int x, int y)
{
	if(x > 0)
		gameobj_set_facing(player_obj, DIRECTION_EAST);
	else if(x < 0)
		gameobj_set_facing(player_obj, DIRECTION_WEST);
	else if(y > 0)
		gameobj_set_facing(player_obj, DIRECTION_SOUTH);
	else if(y < 0)
		gameobj_set_facing(player_obj, DIRECTION_NORTH);
}



// play one of the player's animations
void playerobj_play_anim(PlayerAnimID pid) 
{

	int anim_flags = ANIM_FLAG_LOOPING;
	gameobj_set_anim_data(player_obj, player_anims[pid], anim_flags);
	gameobj_play_anim(player_obj);
}


void playerobj_action_primary()
{
	tongue_extend();
	// perform an action update
	action_update();
}

// perform the B press action
void playerobj_action_secondary()
{
	tongue_retract();
	// perform an action update
	action_update();
}


// returns the current height of the player's hop arc
int playerobj_current_hop_height()
{
	return hop_offset;
}

// checks whether or not a given GameObj is the PlayerObj
bool gameobj_is_player(GameObj *obj)
{
	return (obj == player_obj && obj != NULL);
}