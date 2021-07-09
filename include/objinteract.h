#ifndef OBJINTERACT_H
#define OBJINTERACT_H

#include "gameobj.h"





void objint_push_gameobj(GameObj *obj, int push_dir);

void objint_collect(GameObj *target, GameObj *instigator);
void objint_step_on(GameObj *target, GameObj *instigator);

void objint_deal_damage(GameObj *target, GameObj *instigator);


GameObj *intobj_create_coin_at_position(int x, int y);
GameObj *intobj_create_crate_at_position(int x, int y);

GameObj *floorobj_create_spikes_at_position(int x, int y);


#endif //OBJINTERACT_H