#include "objhistory.h"
#include "gameobj.h"
#include "playerobj.h"
#include "game.h"
#include "map.h"

// camera.c
extern void camera_update_pos();
// ui.c
extern void set_action_count(int count);
// map.c
extern void map_clear_contents();

void set_game_to_turn(int new_turns_ago);
void set_obj_to_turn(ObjHistory *history, int turns_ago);
void clear_obj_future(ObjHistory *history);


inline void link_history(ObjHistory *hist, GameObj *obj){
	if(obj != NULL)
		obj->hist = hist;
	if(hist != NULL)
		hist->game_obj = obj;
};

inline void unlink_history(ObjHistory *hist, GameObj *obj){
	if(obj != NULL)
		obj->hist = NULL;
	if(hist != NULL)
		hist->game_obj = NULL;
};



ObjHistory obj_history_list[OBJ_HISTORY_MAX];		// history of all gameobjs in current scene
static int free_history = 0;						// marker for first free slot in history array


static int game_turns_elapsed;						// how many turns have passed since the game started
static int current_turns_ago;						// measure of how far back in time we are currently rewound



//////////////////
/// ObjHistory ///
//////////////////

// initialize ObjHistory list
void objhistory_init()
{
	clear_all_obj_history();
}

// grabs a free ObjHistory from the list and links it to a provided GameObj
ObjHistory *register_obj_history(GameObj *obj)
{
	ObjHistory *obj_hist = &obj_history_list[free_history];
	// link to gameobj
	link_history(obj_hist, obj);

	free_history++;
	return obj_hist;
}

// returns the ObjHistory at a given index
ObjHistory *get_obj_history(int index)
{
	return  &obj_history_list[index];
}

// update the history of an obj at the end of a turn
void update_obj_history(ObjHistory *history, int facing, int tpos_x, int tpos_y)
{
	if(facing < 0)															//provide a negative facing value to repeat last turn's value
		facing = history->facing_history;
	facing &= 3;															//zero out any irrelevant bits
	history->facing_history = (history->facing_history<<2) | facing;		//shift the history up 2 bits, then add in the new facing value

	int tile_id = get_tile_id(tpos_x,tpos_y);
	for(int i = HISTORY_TURN_MAX-1; i > 0; i--)
	{
		history->tile_history[i] = history->tile_history[i-1];
	}
	history->tile_history[0] = tile_id;
}

// clear and reset the history of an obj
void clear_obj_history(ObjHistory *history)
{
	// unlink from gameobj
	unlink_history(history, history->game_obj);
	history->facing_history = 0;
	for(int i = 0; i < HISTORY_TURN_MAX; i++)
		history->tile_history[i] = -1;
}

// clear and reset all obj histories
void clear_all_obj_history()
{
	for(int i = 0; i < OBJ_HISTORY_MAX; i++)
	{
		clear_obj_history(&obj_history_list[i]);
	}
	free_history = 0;
}




///////////////////////
/// History Reading ///
///////////////////////


// returns the tile id of an objs position X turns ago 
int history_get_tile_id_at_time(ObjHistory *history, int turns_ago)
{
	// if invalid turn supplied 
	if(turns_ago < 0 || turns_ago >= HISTORY_TURN_MAX)
		return -1;

	return history->tile_history[turns_ago];
}

// returns the direction an obj was facing X turns ago
int history_get_facing_at_time(ObjHistory *history, int turns_ago)
{
	// if invalid turn supplied 
	if(turns_ago < 0 || turns_ago >= HISTORY_TURN_MAX)
		return 0;

	u32 facing = history->facing_history;
	u32 mask = 3 << (2*turns_ago);			// create a bit mask
	facing &= mask;							// isolate relevant bits
	facing >>= (2*turns_ago);				// convert to a sensible number
	return facing;
}



////////////////////////
/// Time Shenanigans ///
////////////////////////

// rewind the clock X number of turns 
void history_step_back(int turn_count)
{
	int new_turns_ago = current_turns_ago + turn_count;
	// make sure we dont go farther back than the history limit or before turn 1
	if(new_turns_ago >= HISTORY_TURN_MAX)
		new_turns_ago = (HISTORY_TURN_MAX - 1);
	if(new_turns_ago > game_turns_elapsed)
		new_turns_ago = (game_turns_elapsed);

	set_game_to_turn(new_turns_ago);
	// finalize new position in past
	current_turns_ago = new_turns_ago;
	// update action counter display
	set_action_count(game_turns_elapsed - new_turns_ago); 
}

// jump forward X number of turns
void history_step_forward(int turn_count)
{
	int new_turns_ago = current_turns_ago - turn_count;
	// make sure we dont go farther forward than the present
	if(new_turns_ago < 0)
		new_turns_ago = 0;

	set_game_to_turn(new_turns_ago);
	// finalize new position in past
	current_turns_ago = new_turns_ago;
	// update action counter display
	set_action_count(game_turns_elapsed - new_turns_ago); 
}

// return to the present if the player has not moved yet
void history_return_to_present()
{

	set_game_to_turn(0);
	current_turns_ago = 0;
	turn_count_set(current_turns_ago);
}

// set the game to a turn
void set_game_to_turn(int turns_ago)
{
	
	// i guess???
	map_clear_contents();

	// move all objects to their proper positions for the selected turn
	for(int i = 0; i < OBJ_HISTORY_MAX; i++)
	{
		set_obj_to_turn(&obj_history_list[i], turns_ago);
	}
	// update the camera to keep the player in bounds 
	camera_update_pos();

	// update turn counter
	set_action_count(game_turns_elapsed - turns_ago);
}

// set a GameObj to a specific turn in its history
void set_obj_to_turn(ObjHistory *history, int new_turns_ago)
{
	if(history->game_obj == NULL)
		return;
	if(objprop_is_time_immune(history->game_obj))
		return;

	int tile_id = history_get_tile_id_at_time(history, current_turns_ago);
	// clear old tiles
	//remove_tile_contents_by_id(history->game_obj, tile_id);
	//remove_tile_contents_by_id(history->game_obj, history->game_obj->tile_id);
	// enter new tile
	tile_id = history_get_tile_id_at_time(history, new_turns_ago);
	gameobj_set_tile_pos_by_id(history->game_obj, tile_id);
	
	int facing = history_get_facing_at_time(history, new_turns_ago);
	gameobj_set_facing(history->game_obj, facing);

	if(history->game_obj->anim != NULL)
	{
		history->game_obj->anim->cur_frame = 0;
	}
}

// clear the future and progress with a new timeline
void history_clear_future()
{
	
	for(int i = 0; i < OBJ_HISTORY_MAX; i++)
	{
		clear_obj_future(&obj_history_list[i]);
	}
	

	// update the camera to keep the player in bounds 
	camera_update_pos();

	//update turn tracker
	game_turns_elapsed -= current_turns_ago;
	current_turns_ago = 0;
}

// clear the future of a single gameobj
void clear_obj_future(ObjHistory *history)
{
	if(history->game_obj == NULL)
		return;
	
	if(objprop_is_time_immune(history->game_obj))
		return;

	// push histories forward again
	history->facing_history = (history->facing_history>>(2*current_turns_ago));


	int hmax = HISTORY_TURN_MAX-1;
	history->tile_history[hmax] = history->tile_history[hmax-current_turns_ago];
	for(int i = 0; i < (HISTORY_TURN_MAX-current_turns_ago); i++)
	{
		history->tile_history[i] = history->tile_history[i+current_turns_ago];
	}
	for(int i = (HISTORY_TURN_MAX-current_turns_ago); i < HISTORY_TURN_MAX; i++)
	{
		history->tile_history[i] = -1;		// avoid messiness by setting (ideally unreachable)	history to -1 
	}
}


// update history of all objs after an action is performed
void history_update_all()
{
	for(int i = 0; i < free_history; i++)
	{
		ObjHistory *hist = &obj_history_list[i];
		if(hist->game_obj == NULL)
			continue;
		int tile_x = hist->game_obj->tile_pos.x;
		int tile_y = hist->game_obj->tile_pos.y;
		update_obj_history(hist, gameobj_get_facing(hist->game_obj), tile_x, tile_y);
	}

}


//////////////////
/// Turn Count ///
//////////////////

// get how many turns have elapsed
int turn_count_get()
{
	return game_turns_elapsed;
}

// set the turn count
void turn_count_set(int turn_count)
{
	game_turns_elapsed = turn_count;
	set_action_count(game_turns_elapsed - current_turns_ago);
}

void turn_count_increment()
{
	game_turns_elapsed++;
	set_action_count(game_turns_elapsed - current_turns_ago);
}

void turn_count_decrement()
{
	game_turns_elapsed--;
	set_action_count(game_turns_elapsed - current_turns_ago);
}
