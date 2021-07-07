#ifndef PLAYERTONGUE_H
#define PLAYERTONGUE_H

#include "gameobj.h"

void tongue_init(GameObj *owner);			// initialize tongue
void tongue_set_owner(GameObj *owner);		// set the GameObj (player) this tongue belongs to
void tongue_update();						

void tongue_extend();						// open mouth and send out tongue
void tongue_retract();						// pull the tongue back into the mouth (all the way)
void tongue_contract();						// squish the tongue back into the mouth (one tile)
void tongue_store();						// hides the tongue sprites and resets it in the mouth
void tongue_detach_obj();					// detach the currently attached obj, if it exists
void tongue_detach_tile();
void tongue_detach();
GameObj *tongue_get_attached_object();		// check what, if anything, is attached to the tongue
bool check_tongue_out();					// check if the tongue is currently extended

#endif //PLAYERTONGUE_H