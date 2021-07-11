#ifndef OBJINTERACT_H
#define OBJINTERACT_H

#include "gameobj.h"





void objint_push_gameobj(GameObj *obj, int push_dir);
bool objint_check_floor_tile(GameObj *obj, int tile_x, int tile_y);
void gameobj_fall(GameObj *obj, int tile_x, int tile_y);

void objint_collect(GameObj *target, GameObj *instigator);
void objint_step_on(GameObj *target, GameObj *instigator);

void objint_deal_damage(GameObj *target, GameObj *instigator);


GameObj *intobj_create_coin_at_position(int x, int y);
GameObj *intobj_create_crate_at_position(int x, int y);

GameObj *floorobj_create_victory_tile_at_position(int x, int y);
GameObj *floorobj_create_spikes_at_position(int x, int y);


#endif //OBJINTERACT_H