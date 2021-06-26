#ifndef FROGTONGUE_H
#define FROGTONGUE_H

#include "gameobj.h"

void tongue_init(GameObj *owner);
void tongue_set_owner(GameObj *owner);
void tongue_update();

void tongue_extend();
void tongue_retract();
void tongue_store();
void tongue_attach_obj(GameObj *target);
GameObj *tongue_get_attached_object();
bool check_tongue_out();

#endif //FROGTONGUE_H