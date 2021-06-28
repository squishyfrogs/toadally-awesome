#ifndef OBJINTERACT_H
#define OBJINTERACT_H

#include "gameobj.h"


void objint_push_gameobj(GameObj *obj, int push_dir);
void objint_collect(GameObj *obj);



GameObj *intobj_create_coin_at_position(int x, int y);
GameObj *intobj_create_crate_at_position(int x, int y);


#endif //OBJINTERACT_H