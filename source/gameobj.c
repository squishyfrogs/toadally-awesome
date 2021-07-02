#include <string.h>
#include <tonc.h>
#include "gameobj.h"
#include "objinteract.h"
#include "memory.h"
#include "game.h"
#include "direction.h"
#include "objhistory.h"
#include "animation.h"
#include "map.h"


#define ATTR_COUNT 128			// max number of attrs 
#define OBJ_COUNT 128			// max number of objs

#define SPR_OFF_Y_DEFAULT 2		// default sprite offset (to make sprites sit on bg)


GameObj *create_gameobj_with_id(int obj_id);

void gameobj_update_all();
void gameobj_update(GameObj *obj);
void gameobj_update_movement(GameObj *obj);
void gameobj_update_anim_all();
void gameobj_push_all_updates();

void gameobj_init_all();

void gameobj_update_pos(GameObj *obj);
void gameobj_update_spr_tile_id(GameObj *obj);
void gameobj_push_changes(GameObj *obj);

extern void obj_history_init();
extern Vector2 get_world_offset();

GameObj obj_list[OBJ_COUNT];
OBJ_ATTR objattr_buffer[OBJ_COUNT];







//////////////////////
/// INIT FUNCTIONS ///
//////////////////////

// necessary prep for the GameObj system
void gameobj_init_all()
{
	
	//hides all sprites
	oam_init(objattr_buffer, ATTR_COUNT);

	obj_history_init();
}

void gameobj_erase_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_erase(&obj_list[i]);
	}
	OAM_CLEAR();
	gameobj_hide_all();
}

////////////////////////
/// UPDATE FUNCTIONS ///
////////////////////////

// update all GameObj motion/interaction (once per vsync)
void gameobj_update_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_update(&obj_list[i]);
	}
}

// update a single GameObj
void gameobj_update(GameObj *obj)
{
	if(gameobj_is_moving(obj))
		gameobj_update_movement(obj);
}


// update all GameObj animations (once per (vsync / anim_speed))
void gameobj_update_anim_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		if(obj_list[i].anim.anim_data != NULL)
			anim_update(&obj_list[i].anim);
	}
}

// update all GameObjs (once per vsync)
void gameobj_push_all_updates()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_push_changes(&obj_list[i]);
	}

	// copy all changes into oam memory
	oam_copy(oam_mem, objattr_buffer, ATTR_COUNT);		// copy changes up to obj count
}




/////////////////////////
/// GAMEOBJ FUNCTIONS ///
/////////////////////////


// generate a blank GameObj with a given ID
GameObj *create_gameobj_with_id(int obj_id)
{
	GameObj *obj = &obj_list[obj_id];
	obj->in_use = 1;
	obj->obj_id = obj_id;
	obj->attr = &objattr_buffer[obj_id];
	obj->pal_bank_id = 0;
	obj->spr_tile_id = 0;
	obj->layer_priority = LAYER_GAMEOBJ;
	obj->spr_shape = ATTR0_SQUARE;
	obj->spr_size = ATTR1_SIZE_16x16;	// assume 16x6 for default bc I really doubt anyone is regularly making 16x8 objects 

	obj->tile_pos.x = 0;
	obj->tile_pos.y = 0;

	obj->hist = NULL;
	anim_clear(&obj->anim);
	//obj->anim = anim_create(ANIM_OFFSET_16x16, 1, 0);	// default offset for 16x16 sprite size

	return obj;
}


// initialize a GameObj and return it to the caller
GameObj *gameobj_init()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		if(obj_list[i].in_use == 0)
		{
			return create_gameobj_with_id(i);
		}
	}
	return NULL;
}


// initialize a GameObj in detail
GameObj *gameobj_init_full(u16 layer_priority, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 spr_tile_id, int x, int y, u16 properties)
{
	GameObj *obj = gameobj_init();

	obj->layer_priority = layer_priority;
	obj->spr_shape = attr0_shape;
	obj->pal_bank_id = palbank;
	obj->spr_size = attr1_size;
	obj->spr_tile_id = spr_tile_id;
	obj->obj_properties = properties;

	// if a sprite is FIXED_POS, init differently 
	if(properties & OBJPROP_FIXED_POS)
	{
		// assume it doesn't want an ObjHistory or the default offset, which is mostly intended for the map objs
		obj->hist = NULL;
		// FIXED_POS objs only use pixel pos
		obj->tile_pos.x = -1;
		obj->tile_pos.y = -1;
		obj->pixel_pos.x = x;
		obj->pixel_pos.y = y;
	}
	else
	{
		// history and offset 
		register_obj_history(obj);
		gameobj_set_sprite_offset(obj, 0, SPR_OFF_Y_DEFAULT);
		// set position as tile pos for non-FIXED_POS objs
		obj->pixel_pos.x = 0;
		obj->pixel_pos.y = 0;
		obj->tile_pos.x = x;
		obj->tile_pos.y = y;
	}

	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->spr_tile_id));
	gameobj_update_pos(obj);

	return obj;
}

// duplicate a GameObj into another slot in memory
GameObj *gameobj_duplicate(GameObj *src)
{
	GameObj *obj = gameobj_init();
	return gameobj_clone(obj, src);
}

// copy all attributes of a GameObj into another existing GameObj
GameObj *gameobj_clone(GameObj *dest, GameObj *src)
{
	if(dest == src || dest == NULL || src == NULL)
		return dest;

	//dest->attr = src->attr;

	dest->layer_priority = src->layer_priority;
	dest->spr_tile_id = src->spr_tile_id;
	dest->pal_bank_id = src->pal_bank_id;
	dest->layer_priority = src->layer_priority;
	dest->spr_shape = src->spr_shape;
	dest->spr_size = src->spr_size;

	dest->tile_pos = src->tile_pos;
	dest->pixel_pos = src->pixel_pos;
	dest->spr_off = src->spr_off;

	dest->obj_properties = src->obj_properties;
	// copy anim data and flags, but keep frame at 0 
	dest->anim.anim_data = src->anim.anim_data;
	dest->anim.flags = src->anim.flags;
	// do not copy these -- remake from scratch
	//obj->hist = src->hist;

	obj_set_attr(dest->attr, dest->spr_shape, dest->spr_size, (ATTR2_PALBANK(dest->pal_bank_id) | dest->spr_tile_id));
	gameobj_update_pos(dest);

	return dest;
}

// wipe all attributes of a GameObj and mark it as unused
void gameobj_erase(GameObj *obj)
{
	obj_hide(&objattr_buffer[obj->obj_id]);
	obj_set_attr(&objattr_buffer[obj->obj_id], 0, 0, 0);
	obj->in_use = 0;
	obj->obj_id = 0;
	obj->pal_bank_id = 0;
	obj->spr_tile_id = 0;
	obj->layer_priority = 0;
	obj->spr_shape = 0;
	obj->spr_size = 0;

	obj->tile_pos.x = 0;
	obj->tile_pos.y = 0;

	if(obj->hist != NULL)
		clear_obj_history(obj->hist);
	obj->hist = NULL;
	anim_clear(&obj->anim);
	
}


// set a GameObj's attributes
void gameobj_update_attr(GameObj *obj)
{
	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->spr_tile_id));
	gameobj_update_pos(obj);
}

// set a GameObj's attributes
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 spr_tile_id, int x, int y, u16 properties)
{
	obj->spr_shape = attr0_shape;
	obj->spr_size = attr1_size;
	obj->pal_bank_id = palbank;
	obj->spr_tile_id = spr_tile_id;
	obj->obj_properties = properties;

	// if a sprite is FIXED_POS, update differently 
	if(properties & OBJPROP_FIXED_POS)
	{
		// FIXED_POS objs only use pixel pos
		obj->pixel_pos.x = x;
		obj->pixel_pos.y = y;
	}
	else
	{
		// set position as tile pos for non-FIXED_POS objs
		obj->tile_pos.x = x;
		obj->tile_pos.y = y;
	}

	u32 tid = obj->spr_tile_id + (obj->anim.anim_data->tile_offset * obj->anim.cur_frame);
	obj_set_attr(obj->attr, attr0_shape, attr1_size, (ATTR2_PALBANK(palbank) | tid));
	gameobj_update_pos(obj);
}

// set a GameObj's properties
void gameobj_set_property_flags(GameObj *obj, u16 properties)
{
	obj->obj_properties = properties;
}

// return the matching bits in a GameObj's property flags
u16 gameobj_check_properties(GameObj *obj, u16 properties)
{
	if(obj == NULL)
		return 0;
	return obj->obj_properties & properties;
}

// unhide a GameObj
void gameobj_unhide(GameObj *obj)
{
	obj->obj_properties &= ~OBJPROP_HIDDEN;
	obj_unhide(obj->attr,DCNT_MODE0);
}

// hide a GameObj
void gameobj_hide(GameObj *obj)
{
	obj->obj_properties |= OBJPROP_HIDDEN;
	obj_hide(obj->attr);
}


////////////////
/// Graphics ///
////////////////

// set a GameObj's animation info
void gameobj_set_anim_data(GameObj *obj, AnimationData *anim_data, u8 flags)
{
	
	anim_init(&obj->anim, anim_data, flags);
	
	gameobj_update_spr_tile_id(obj);
	
}


void gameobj_play_anim(GameObj *obj)
{
	anim_play(&obj->anim);
}

// set a GameObj's sprite offset
void gameobj_set_sprite_offset(GameObj *obj, int x, int y)
{
	obj->spr_off.x = x;
	obj->spr_off.y = y;
	gameobj_update_pos(obj);
}

// flip an object horizontally across the vertical axis
void gameobj_flip_h(GameObj *obj)
{
	obj->attr->attr1 ^= ATTR1_HFLIP;
}

// flip an object vertically across the horizontal axis
void gameobj_flip_v(GameObj *obj)
{
	obj->attr->attr1 ^= ATTR1_VFLIP;
}

// set the horizontal and vertical flip state 
void gameobj_set_flip(GameObj *obj, bool flip_h, bool flip_v)
{
	if(flip_h)
		obj->attr->attr1 |= ATTR1_HFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_HFLIP;

	if(flip_v)
		obj->attr->attr1 |= ATTR1_VFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_VFLIP;
}

// set the horizontal flip state
void gameobj_set_flip_h(GameObj *obj, bool flip_h)
{
	if(flip_h)
		obj->attr->attr1 |= ATTR1_HFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_HFLIP;
}

// set the vertical flip state
void gameobj_set_flip_v(GameObj *obj, bool flip_v)
{
	if(flip_v)
		obj->attr->attr1 |= ATTR1_VFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_VFLIP;
}


// updates a GameObj's sprite after changing animation or direction
void gameobj_update_spr_tile_id(GameObj *obj)
{
	if(obj == NULL || obj->anim.anim_data == NULL)
		return;
	int dir = gameobj_get_facing(obj);
	if(!(obj->anim.flags & ANIM_FLAG_ASYMMETRIC))
	{
		if(dir == DIRECTION_WEST)
		{
			dir = DIRECTION_EAST;
			gameobj_set_flip_h(obj, true);
		}
		else
		{
			gameobj_set_flip_h(obj, false);
		}
	}
	
	//obj->spr_tile_id = obj->anim->tile_start + (obj->anim->cur_frame * obj->anim->tile_offset) + (dir * obj->anim->facing_offset * obj->anim->tile_offset);
	obj->spr_tile_id = obj->anim.anim_data->tile_start + (dir * obj->anim.anim_data->facing_offset * obj->anim.anim_data->tile_offset);
	gameobj_push_changes(obj);
}


////////////////
/// Position ///
////////////////

// set a GameObj's tile position
void gameobj_set_tile_pos(GameObj *obj, int x, int y)
{
	obj->tile_pos.x = x;
	obj->tile_pos.y = y;
	gameobj_update_pos(obj);
}

// set a GameObj's tile position
void gameobj_set_tile_pos_by_id(GameObj *obj, int tile_id)
{
	if(tile_id < 0 || tile_id >= MAP_SIZE)
		return;
	int x = tile_id % MAP_SIZE_X;
	int y = tile_id / MAP_SIZE_X;
	gameobj_set_tile_pos(obj, x, y);
}

// set a GameObj's position to new values
void gameobj_set_pixel_pos(GameObj *obj, int x, int y)
{
	obj->pixel_pos.x = x;
	obj->pixel_pos.y = y;
	gameobj_update_pos(obj);
}

// translate a GameObj by (x,y)
void gameobj_change_pixel_pos(GameObj *obj, int move_x, int move_y)
{
	obj->pixel_pos.x += move_x;
	obj->pixel_pos.y += move_y;
	gameobj_update_pos(obj);
}

// update a GameObj's attrs based on its current position
void gameobj_update_pos(GameObj *obj)
{
	Vector2 w_off = get_world_offset();
	int pos_x = (obj->tile_pos.x * GAME_TILE_SIZE) + obj->pixel_pos.x - obj->spr_off.x - w_off.x;
	int pos_y = (obj->tile_pos.y * GAME_TILE_SIZE) + obj->pixel_pos.y - obj->spr_off.y - w_off.y;
	if(obj->obj_properties & OBJPROP_FIXED_POS)
		obj_set_pos(obj->attr, obj->pixel_pos.x - obj->spr_off.x, obj->pixel_pos.y - obj->spr_off.y);
	else
		obj_set_pos(obj->attr, pos_x, pos_y);
}


// get the pixel position of a GameObj as a Vector2
Vector2 gameobj_get_pixel_pos(GameObj *obj)
{
	Vector2 v;
	if(gameobj_check_properties(obj, OBJPROP_FIXED_POS))
	{
		v.x = obj->pixel_pos.x;
		v.y = obj->pixel_pos.y;
	}
	else
	{
		v.x = (obj->tile_pos.x * GAME_TILE_SIZE) + obj->pixel_pos.x;
		v.y = (obj->tile_pos.y * GAME_TILE_SIZE) + obj->pixel_pos.y;
	}
	return v;
}

// remove obj from old tile, update tile + pixel vectors, assign obj to new tile
void gameobj_update_current_tile(GameObj *obj)
{
	int x = obj->pixel_pos.x / GAME_TILE_SIZE;
	int y = obj->pixel_pos.y / GAME_TILE_SIZE;

	remove_tile_contents(obj, obj->tile_pos.x, obj->tile_pos.y);

	obj->tile_pos.x += x;
	obj->pixel_pos.x -= (x * GAME_TILE_SIZE);

	obj->tile_pos.y += y;
	obj->pixel_pos.y -= (y * GAME_TILE_SIZE);


	// check tile for collectibles
	GameObj *contents = get_tile_contents(obj->tile_pos.x, obj->tile_pos.y);
	if(gameobj_check_properties(contents, OBJPROP_PICKUP))
	{
		objint_collect(contents, obj);
	}
	
	set_tile_contents(obj, obj->tile_pos.x, obj->tile_pos.y);

	// check tile for floor objs
	GameObj *floor_obj = get_tile_floor_contents(obj->tile_pos.x, obj->tile_pos.y);
	if(floor_obj != NULL)
	{
		objint_step_on(floor_obj, obj);
	}
}


//////////////
/// Facing ///
//////////////

void gameobj_set_facing(GameObj *obj, int facing)
{
	facing = (facing & 0x0003) << OBJPROP_FACING_BIT_OFFSET;
	obj->obj_properties = obj->obj_properties & ~(0x0003 << OBJPROP_FACING_BIT_OFFSET);
	obj->obj_properties = obj->obj_properties | facing;

	gameobj_update_spr_tile_id(obj);
}

int gameobj_get_facing(GameObj *obj)
{
	int props = obj->obj_properties;
	props = (props >> OBJPROP_FACING_BIT_OFFSET) & 0x0003;
	return props;
}



////////////////
/// Move Dir ///
////////////////

void gameobj_set_move_dir(GameObj *obj, int move_dir)
{
	move_dir = (move_dir & 0x0003) << OBJPROP_MOVING_BIT_OFFSET;
	obj->obj_properties = obj->obj_properties & ~(0x0003 << OBJPROP_MOVING_BIT_OFFSET);
	obj->obj_properties = obj->obj_properties | move_dir;

	gameobj_update_spr_tile_id(obj);
}

int gameobj_get_move_dir(GameObj *obj)
{
	int props = obj->obj_properties;
	props = (props >> OBJPROP_MOVING_BIT_OFFSET) & 0x0003;
	return props;
}

void gameobj_set_moving(GameObj *obj, bool moving, int move_dir)
{
	if(obj == NULL)
		return;
	if(moving)
	{
		gameobj_set_move_dir(obj, move_dir);
		obj->obj_properties |= OBJPROP_MOVING;
	}
	else
		obj->obj_properties &= ~OBJPROP_MOVING;
}

void gameobj_set_moving_vec(GameObj *obj, bool moving, Vector2 move_dir)
{
	if(obj == NULL)
		return;
	if(moving)
	{
		gameobj_set_move_dir(obj, vec_to_dir(move_dir));
		obj->obj_properties |= OBJPROP_MOVING;
	}
	else
		obj->obj_properties &= ~OBJPROP_MOVING;
}

bool gameobj_is_moving(GameObj *obj)
{
	return (obj->obj_properties & OBJPROP_MOVING) != 0;
}

void gameobj_update_movement(GameObj *obj)
{
	if(gameobj_is_player(obj))
		return;
	if(!gameobj_is_moving(obj))
		return;
	Vector2 mov = dir_to_vec(gameobj_get_move_dir(obj));
	Vector2 offset;
	// TODO: add move speed or something here
	offset.x = obj->pixel_pos.x + mov.x;
	offset.y = obj->pixel_pos.y + mov.y;
	if((offset.x >= GAME_TILE_SIZE) || (offset.x <= -GAME_TILE_SIZE))
		mov.x = 0;
	if((offset.y >= GAME_TILE_SIZE) || (offset.y <= -GAME_TILE_SIZE))
		mov.y = 0;

	gameobj_set_pixel_pos(obj, offset.x, offset.y);	
	
	if(mov.x == 0 && mov.y == 0)
	{
		gameobj_update_current_tile(obj);
		gameobj_set_moving(obj, false, 0);
	}
}

// returns true if no gameobjs are in motion (used to check for turn end)
bool gameobj_all_at_rest()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		if(gameobj_is_moving(&obj_list[i]))
			return false;
	}
	return true;
}



// push changes to a GameObj's position, animation, and palette
void gameobj_push_changes(GameObj *obj)
{
	u32 spr_tile_id = obj->spr_tile_id;
	if(obj->anim.anim_data != NULL)
		spr_tile_id += (obj->anim.anim_data->tile_offset * obj->anim.cur_frame);

	obj->attr->attr2 = ATTR2_BUILD(
		spr_tile_id,
		obj->pal_bank_id,
		obj->layer_priority
	);
	gameobj_update_pos(obj);
}




// hide all GameObjs
void gameobj_hide_all()
{
	obj_hide_multi(&objattr_buffer[0], OBJ_COUNT);
}

// unhide all GameObjs
void gameobj_unhide_all()
{
	obj_unhide_multi(&objattr_buffer[0], DCNT_MODE0, OBJ_COUNT);
}