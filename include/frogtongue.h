#ifndef FROGTONGUE_H
#define FROGTONGUE_H

#include "gameobj.h"

void tongue_init(GameObj *owner);
void tongue_set_owner(GameObj *owner);
void tongue_extend();
void tongue_retract();
void tongue_attach_obj();
void tongue_update();


#endif //FROGTONGUE_H