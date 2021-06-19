#include "gameobj.h"
#include "effects.h"

void push_gameobj(GameObj *obj, int push_dir);

// push a game object
void push_gameobj(GameObj *obj, int push_dir)
{
	gameobj_set_moving(obj, true, push_dir);
	create_effect_at_position(ET_SMOKE, obj->tile_pos.x, obj->tile_pos.y);
	//create_effect_at_position(obj->tile_pos.x, obj->tile_pos.y);
}
