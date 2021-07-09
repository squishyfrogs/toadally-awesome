#ifndef PLAYERTONGUE_H
#define PLAYERTONGUE_H

#include "gameobj.h"

void tongue_init(GameObj *owner);			// initialize tongue
void tongue_set_owner(GameObj *owner);		// set the GameObj (player) this tongue belongs to
void tongue_update();						

void tongue_extend();						// open mouth and send out tongue
void tongue_retract();						// pull the tongue back into the mouth (all the way)
bool tongue_stretch();						// increase the length of the already extended tongue by one tile, returns false if already max length
void tongue_contract();						// squish the tongue back into the mouth (one tile)
void tongue_store();						// hides the tongue sprites and resets it in the mouth
void tongue_detach_obj();					// detach the currently attached obj, if it exists
void tongue_detach_tile();
void tongue_detach();
GameObj *tongue_get_attached_object();		// check what, if anything, is attached to the tongue
Vector2 tongue_get_attached_tile();			// check what tile the tongue is attached to
bool check_tongue_out();					// check if the tongue is currently extended
bool check_tongue_attached();				// check if the tongue is currently attached to an obj or tile

#endif //PLAYERTONGUE_H