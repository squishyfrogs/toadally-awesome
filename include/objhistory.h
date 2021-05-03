#ifndef OBJHISTORY_H
#define OBJHISTORY_H

#include <tonc.h>
#include "gameobj.h"


#define HISTORY_TURN_MAX 16	// maximum turns to remember
#define OBJ_HISTORY_MAX 32	// how many objs to remember

// history of a GameObj's movement and facing direction
typedef struct struct_ObjHistory{
	GameObj *game_obj;
	uint32_t facing_history;				// facing data stored as 16 pairs of bits to save space (see directions.h for values)
	int tile_history[HISTORY_TURN_MAX];		// position data stored as a series of ints (referring to tileID)
} ObjHistory;


void update_obj_history(ObjHistory *history, int facing, int pos_x, int pos_y);
void clear_obj_history(ObjHistory *history);		// clear the history of an obj

#endif