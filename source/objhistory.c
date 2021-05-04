#include "objhistory.h"

extern int get_tile_id(int x, int y);


ObjHistory obj_history_list[OBJ_HISTORY_MAX];		// history of all gameobjs in current scene
static int free_history = 0;						// marker for first free slot in history array



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
int get_tile_id_at_time(ObjHistory *history, int turns_ago)
{
	// if invalid turn supplied 
	if(turns_ago < 0 || turns_ago >= HISTORY_TURN_MAX)
		return 0;

	int turn = HISTORY_TURN_MAX - 1 - turns_ago;

	return history->tile_history[turn];
}


int get_facing_at_time(ObjHistory *history, int turns_ago)
{
	// if invalid turn supplied 
	if(turns_ago < 0 || turns_ago >= HISTORY_TURN_MAX)
		return 0;


	uint32_t facing = history->facing_history;
	facing &= (3<<turns_ago);			// isolate relevant bits
	facing >>= turns_ago;				// convert to relevant number
	return facing;
}