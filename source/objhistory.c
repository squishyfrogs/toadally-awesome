#include "objhistory.h"

extern int get_tile_id(int x, int y);


void update_obj_history(ObjHistory *history, int facing, int pos_x, int pos_y)
{
	facing &= 3;															//zero out any stray bits just in case
	history->facing_history = (history->facing_history<<2) | facing;		//shift the history up 2 bits, then add in the new facing value

	int tile_id = get_tile_id(pos_x,pos_y);
	for(int i = 0; i < HISTORY_TURN_MAX-1; i++)
	{
		history->tile_history[i] = history->tile_history[i+1];
	}
	history->tile_history[HISTORY_TURN_MAX-1] = tile_id;
}


void clear_obj_history(ObjHistory *history)
{
	history->game_obj = NULL;
	history->facing_history = 0;
	for(int i = 0; i < HISTORY_TURN_MAX; i++)
		history->tile_history[i] = 0;
}

