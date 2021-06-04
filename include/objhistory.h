#ifndef OBJHISTORY_H
#define OBJHISTORY_H

#include <tonc.h>
#include "gameobj.h"


#define HISTORY_TURN_MAX 16	// maximum turns to remember
#define OBJ_HISTORY_MAX 32	// how many objs to remember

// history of a GameObj's movement and facing direction
typedef struct struct_ObjHistory{
	u8 in_use;																			// if zero, history is free to be overwritten
	struct struct_GameObj *game_obj;
	uint32_t facing_history;															// facing data stored as 16 pairs of bits to save space (see directions.h for values)
	int tile_history[HISTORY_TURN_MAX];													// position data stored as a series of ints (referring to tileID)
} ObjHistory;

void objhistory_init();																	// initialize ObjHistory list
ObjHistory *register_obj_history(GameObj *obj);											// returns a free ObjHistory from the list and assigns a GameObj to it
ObjHistory *get_obj_history(int index);													// returns the ObjHistory at a given index
void update_obj_history(ObjHistory *history, int facing, int tpos_x, int tpos_y);		// pushes changes to the action queue
void clear_obj_history(ObjHistory *history);											// clear and reset the history of an obj
void clear_all_obj_history();															// clear and reset all obj histories

int history_get_tile_id_at_time(ObjHistory *history, int turns_ago);					// returns the tile id of an objs position X turns ago
int history_get_facing_at_time(ObjHistory *history, int turns_ago);						// returns the direction an obj was facing X turns ago

void history_step_back(int turn_count);													// rewind the clock X number of turns
void history_step_forward(int turn_count);												// jump forward X number of turns
void history_return_to_present();														// return to the present if the player has not moved yet
void history_clear_future();															// clear the future and progress with a new timeline

void history_update_all();																// update history of all objs after an action is performed
int turn_count_get();																	// get how many turns have elapsed
void turn_count_set(int turn_count);													// set the turn count
void turn_count_increment();
void turn_count_decrement();

#endif //OBJHISTORY_H