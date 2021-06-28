#include "objinteract.h"
#include "effects.h"
#include "map.h"

#include "sprites/objects/coin.h"
#include "sprites/objects/crate.h"





///////////////////////////
/// Object Interactions ///
///////////////////////////

// push a game object
void objint_push_gameobj(GameObj *obj, int push_dir)
{
	if(gameobj_check_properties(obj, OBJPROP_MOVABLE) != OBJPROP_MOVABLE)
		return;
	gameobj_set_moving(obj, true, push_dir);
	create_effect_at_position(ET_SMOKE, obj->tile_pos.x, obj->tile_pos.y);
	//create_effect_at_position(obj->tile_pos.x, obj->tile_pos.y);
}

// collect a (valid) game object
void objint_collect(GameObj *obj)
{
	if(gameobj_check_properties(obj, OBJPROP_PICKUP) != OBJPROP_PICKUP)
		return;
	// TODO: Apply Collect Effect
	create_effect_at_position(ET_SMOKE, obj->tile_pos.x, obj->tile_pos.y);
	remove_tile_contents(obj, obj->tile_pos.x, obj->tile_pos.y);
	gameobj_erase(obj);
}




////////////////////////////
/// Interactable Objects ///
////////////////////////////


GameObj *intobj_create_crate_at_position(int x, int y)
{
	int c_pal = mem_load_palette(cratePal);
	int c_tile = mem_load_tiles(crateTiles, crateTilesLen);
	GameObj *crate = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_TALL, ATTR1_SIZE_16x32, c_pal, c_tile, 0, 0, OBJPROP_SOLID|OBJPROP_MOVABLE);
	gameobj_set_sprite_offset(crate,0,8);
	place_obj_in_tile(crate, x, y);
	return crate;
}


GameObj *intobj_create_coin_at_position(int x, int y)
{
	int c_pal = mem_load_palette(coinPal);
	int c_tile = mem_load_tiles(coinTiles, coinTilesLen);
	GameObj *coin = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, c_pal, c_tile, 0, 0, OBJPROP_PICKUP);
	gameobj_set_sprite_offset(coin,0,2);
	place_obj_in_tile(coin, x, y);
	AnimationData *coin_anim = animdata_create(c_tile, 4, 4, 0);
	gameobj_set_anim_data(coin, coin_anim, ANIM_FLAG_LOOPING);
	gameobj_play_anim(coin);
	return coin;
}