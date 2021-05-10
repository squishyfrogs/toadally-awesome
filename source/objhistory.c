#include "objhistory.h"

// main.c
extern int current_turn();
// map.c
extern int get_tile_id(int x, int y);
extern void clear_tile_contents_by_id(int tile_id);
// camera.c
extern void camera_find_target(GameObj *target);
// playerobj.c
extern GameObj *get_player_obj();
extern void player_set_tile_by_id(int tile_id);
extern void playerobj_set_facing(int dir);
// ui.c
extern void set_action_count(int count);



void set_player_to_turn(ObjHistory *history, int turns_ago);
void set_obj_to_turn(ObjHistory *history, int turns_ago);
void clear_obj_future(ObjHistory *history);


ObjHistory obj_history_list[OBJ_HISTORY_MAX];		// history of all gameobjs in current scene
static int free_history = 0;						// marker for first free slot in history array

static int current_turns_ago;						// measure of how far back in time we are currently rewound


// initialize ObjHistory list
void objhistory_init()
{
	clear_all_obj_history();
}

// returns a free ObjHistory from the list
ObjHistory *register_obj_history(GameObj *obj)
{
	ObjHistory *obj_hist = &obj_history_list[free_history];
	obj_hist->game_obj = obj;

	free_history++;
	return obj_hist;
}

// returns the ObjHistory at a given index
ObjHistory *get_obj_history(int index)
{
	return  &obj_history_list[index];
}

// update the history of an obj at the end of a turn
void update_obj_history(ObjHistory *history, int facing, int pos_x, int pos_y)
{

	if(facing < 0)															//provide a negative facing value to repeat last turn's value
		facing = history->facing_history;
	facing &= 3;															//zero out any irrelevant bits
	history->facing_history = (history->facing_history<<2) | facing;		//shift the history up 2 bits, then add in the new facing value

	int tile_id = get_tile_id(pos_x,pos_y);
	for(int i = 0; i < HISTORY_TURN_MAX-1; i++)
	{
		history->tile_history[i] = history->tile_history[i+1];
	}
	history->tile_history[HISTORY_TURN_MAX-1] = tile_id;
}


// clear and reset the history of an obj
void clear_obj_history(ObjHistory *history)
{
	history->game_obj = NULL;
	history->facing_history = 0;
	for(int i = 0; i < HISTORY_TURN_MAX; i++)
		history->tile_history[i] = 0;
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
		return 0;

	int turn = (HISTORY_TURN_MAX - 1) - turns_ago;
	return history->tile_history[turn];
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
void history_rewind(int turn_count)
{
	
	int c_turn = current_turn(); 
	// make sure we dont go too far back
	if(current_turns_ago + turn_count >= HISTORY_TURN_MAX)
		turn_count = (HISTORY_TURN_MAX - 1) - current_turns_ago;
	
	
	int new_turns_ago = current_turns_ago + turn_count;
	if(new_turns_ago > c_turn)
		new_turns_ago = c_turn;

	for(int i = 0; i < OBJ_HISTORY_MAX; i++)
	{
		set_obj_to_turn(&obj_history_list[i], new_turns_ago);
	}
	set_player_to_turn(&obj_history_list[0], new_turns_ago);
	// update action counter to display the old turn
	set_action_count(c_turn - new_turns_ago);
	// update the camera to keep the player in bounds 
	camera_find_target(get_player_obj());

	//update turn tracker
	current_turns_ago = new_turns_ago;
}

// return to the present if the player has not moved yet
void history_return_to_present()
{
	int new_turns_ago = 0;

	for(int i = 1; i < OBJ_HISTORY_MAX; i++)
	{
		set_obj_to_turn(&obj_history_list[i], new_turns_ago);
	}
	set_player_to_turn(&obj_history_list[0], new_turns_ago);
	// update action counter to display the present
	set_action_count(current_turn());
	// update the camera to keep the player in bounds 
	camera_find_target(get_player_obj());

	//update turn tracker
	current_turns_ago = 0;
}


// set a GameObj to a specific turn in its history
void set_obj_to_turn(ObjHistory *history, int new_turns_ago)
{
	if(history->game_obj == NULL)
		return;
	if(objprop_is_time_immune(history->game_obj))
		return;

	// clear old tile
	int tile_id = history_get_tile_id_at_time(history, current_turns_ago);
	clear_tile_contents_by_id(tile_id);
	// enter new tile
	tile_id = history_get_tile_id_at_time(history, new_turns_ago);
	gameobj_set_tile_pos_by_id(history->game_obj, tile_id);
	
	int facing = history_get_facing_at_time(history, new_turns_ago);
	gameobj_set_facing(history->game_obj, facing);
}

// TODO: genericize this so it doesnt have to be separate from the gameobj one 
void set_player_to_turn(ObjHistory *history, int new_turns_ago)
{
	if(history->game_obj == NULL)
		return;
	if(objprop_is_time_immune(history->game_obj))
		return;

	int tile_id = history_get_tile_id_at_time(history, new_turns_ago);
	set_action_count(tile_id);
	gameobj_set_tile_pos_by_id(history->game_obj, tile_id);
	player_set_tile_by_id(tile_id);
	int facing = history_get_facing_at_time(history, new_turns_ago);
	playerobj_set_facing(facing);
}

// clear the future and progress with a new timeline
void history_clear_future()
{
	
	for(int i = 0; i < OBJ_HISTORY_MAX; i++)
	{
		clear_obj_future(&obj_history_list[i]);
	}
	

	// update the camera to keep the player in bounds 
	camera_find_target(get_player_obj());

	//update turn tracker
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
	for(int i = (HISTORY_TURN_MAX-1) - current_turns_ago; i > 0; i--)
	{
		history->tile_history[i+current_turns_ago] = history->tile_history[i];
	}
	for(int i = 0; i < current_turns_ago; i++)
	{
		history->tile_history[i] = 0;		// avoid messiness by setting (ideally unreachable)	history to 0
	}
}