#include <string.h>
#include <tonc.h>
#include "game.h"
#include "direction.h"
#include "gameobj.h"
#include "objhistory.h"
#include "animation.h"
#include "map.h"

#define SPR_OFF_Y_DEFAULT 2		// default sprite offset (to make sprites sit on bg)

void gameobj_init_all();
void gameobj_update_anim_all();
void gameobj_push_all_updates();

void gameobj_update_pos(GameObj *obj);
void gameobj_update_facing(GameObj *obj);							// updates a GameObj's sprite after changing direction
void gameobj_push_changes(GameObj *obj);

extern void objhistory_init();


GameObj obj_list[OBJ_COUNT];
OBJ_ATTR objattr_buffer[OBJ_COUNT];
static int free_obj = 0;		//marker of first free obj in the object list
static int free_pal = 0;		//marker of first free palette in VRAM
static int free_tile = 0;		//marker of first free tile in VRAM


extern int world_offset_x;
extern int world_offset_y;



//////////////////////
/// INIT FUNCTIONS ///
//////////////////////

// necessary prep for the GameObj system
void gameobj_init_all()
{
	
	//hides all sprites
	oam_init(objattr_buffer, 128);

	objhistory_init();
}



////////////////////////
/// UPDATE FUNCTIONS ///
////////////////////////


// update all GameObj animations (once per (vsync / anim_speed))
void gameobj_update_anim_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		if(obj_list[i].anim != NULL)
			anim_update(obj_list[i].anim);
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
	oam_copy(oam_mem, objattr_buffer, free_obj);		// only need to copy changes up to obj count
}




////////////////////////
/// MEMORY FUNCTIONS ///
////////////////////////

int mem_load_palette(const ushort *pal_data)
{
	int pal_id = free_pal;
	memcpy(pal_obj_mem + (pal_id*16), pal_data, 32);	// 16 colors per palette, 2 bytes per color
	free_pal++;
	return pal_id;
}

int mem_load_tiles(const ushort *tile_data, int data_len)
{
	//always block 4? 
	int tile_id = free_tile;
	memcpy(&tile_mem[4][tile_id], tile_data, data_len);
	free_tile += (data_len/32); // divide by 32;
	return tile_id;
}


/////////////////////////
/// GAMEOBJ FUNCTIONS ///
/////////////////////////


// initialize a GameObj and return it to the caller
GameObj *init_gameobj()
{
	GameObj *obj = init_gameobj_with_id(free_obj);
	// eventually free_obj will be stuck at 127 - this is intended behavior (for the time being)
	if(free_obj+1 < OBJ_COUNT)
		free_obj++;
	return obj;
}

// initialize a blank GameObj with a given ID
GameObj *init_gameobj_with_id(int obj_id)
{
	GameObj *obj = &obj_list[obj_id];
	obj->obj_id = obj_id;
	obj->attr = &objattr_buffer[obj_id];
	obj->pal_bank_id = 0;
	obj->tile_id = 0;
	obj->layer_priority = LAYER_GAMEOBJ;
	obj->spr_shape = ATTR0_SQUARE;
	obj->spr_size = ATTR1_SIZE_16x16;	// assume 16x6 for default bc I really doubt anyone is regularly making 16x8 objects 

	obj->tile_pos.x = 0;
	obj->tile_pos.y = 0;

	obj->anim = NULL;
	//obj->anim = anim_create(ANIM_OFFSET_16x16, 1, 0);	// default offset for 16x16 sprite size

	return obj;
}

// initialize a GameObj in detail
GameObj *init_gameobj_full(u16 layer_priority, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, u16 properties)
{
	GameObj *obj = init_gameobj();

	obj->layer_priority = layer_priority;
	obj->spr_shape = attr0_shape;
	obj->pal_bank_id = palbank;
	obj->spr_size = attr1_size;
	obj->tile_id = tile_id;
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

	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->tile_id));
	gameobj_update_pos(obj);

	return obj;
}




// set a GameObj's attributes
void gameobj_update_attr(GameObj *obj)
{
	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->tile_id));
	gameobj_update_pos(obj);
}

// set a GameObj's attributes
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, u16 properties)
{
	obj->spr_shape = attr0_shape;
	obj->spr_size = attr1_size;
	obj->pal_bank_id = palbank;
	obj->tile_id = tile_id;
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

	u32 tid = obj->tile_id + (obj->anim->tile_offset * obj->anim->cur_frame);
	obj_set_attr(obj->attr, attr0_shape, attr1_size, (ATTR2_PALBANK(palbank) | tid));
	gameobj_update_pos(obj);
}

// set a GameObj's properties
void gameobj_set_property_flags(GameObj *obj, u16 properties)
{
	obj->obj_properties = properties;
}

// set a GameObj's animation info
void gameobj_set_anim_info(GameObj *obj, u16 frame_count, short tile_offset, int facing_offset, bool looping)
{
	u8 flags = 0;
	if(looping)
		flags |= ANIM_FLAG_LOOPING;
	if(obj->anim == NULL)
		obj->anim = anim_create(obj->tile_id, tile_offset, frame_count, facing_offset, flags);
	else
		anim_set_info(obj->anim, obj->tile_id, tile_offset, frame_count, facing_offset, flags);
}


// set a GameObj's sprite offset
void gameobj_set_sprite_offset(GameObj *obj, int x, int y)
{
	obj->spr_off.x = x;
	obj->spr_off.y = y;
	gameobj_update_pos(obj);
}

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
	int pos_x = (obj->tile_pos.x * GAME_TILE_SIZE) + obj->pixel_pos.x - obj->spr_off.x - world_offset_x;
	int pos_y = (obj->tile_pos.y * GAME_TILE_SIZE) + obj->pixel_pos.y - obj->spr_off.y - world_offset_y;
	if(obj->obj_properties & OBJPROP_FIXED_POS)
		obj_set_pos(obj->attr, obj->pixel_pos.x - obj->spr_off.x, obj->pixel_pos.y - obj->spr_off.y);
	else
		obj_set_pos(obj->attr, pos_x, pos_y);
}


// get the pixel position of a GameObj as a Vector2
Vector2 gameobj_get_pixel_pos(GameObj *obj)
{
	Vector2 v;
	if(obj->obj_properties & OBJPROP_FIXED_POS)
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

// 
void gameobj_update_current_tile(GameObj *obj)
{
	int x = obj->pixel_pos.x / GAME_TILE_SIZE;
	int y = obj->pixel_pos.y / GAME_TILE_SIZE;
	obj->tile_pos.x += x;
	obj->pixel_pos.x -= (x * GAME_TILE_SIZE);

	obj->tile_pos.y += y;
	obj->pixel_pos.y -= (y * GAME_TILE_SIZE);
}

//////////////
/// Facing ///
//////////////

void gameobj_set_facing(GameObj *obj, int facing)
{
	facing = (facing & 0x0003) << 14;
	obj->obj_properties = obj->obj_properties & 0x3FFF;
	obj->obj_properties = obj->obj_properties | facing;

	gameobj_update_facing(obj);
}

int gameobj_get_facing(GameObj *obj)
{
	int props = obj->obj_properties;
	props = (props & 0xC000) >> 14;
	return props;
}

// updates a GameObj's sprite after changing direction
void gameobj_update_facing(GameObj *obj)
{
	if(obj == NULL || obj->anim == NULL)
		return;
	int dir = gameobj_get_facing(obj);
	if(!(obj->anim->flags & ANIM_FLAG_ASYMMETRIC))
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
	obj->tile_id = obj->anim->tile_start + (obj->anim->cur_frame * obj->anim->tile_offset) + (dir * obj->anim->facing_offset * obj->anim->tile_offset);
	gameobj_push_changes(obj);
}


/////////////
/// Flips ///
/////////////

// toggle horizontal flip
void gameobj_flip_h(GameObj *obj)
{
	obj->attr->attr1 ^= ATTR1_HFLIP;
}

// toggle vertical flip
void gameobj_flip_v(GameObj *obj)
{
	obj->attr->attr1 ^= ATTR1_VFLIP;
}

// set flip values for a GameObj
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

// set horizontal flip value for a GameObj
void gameobj_set_flip_h(GameObj *obj, bool flip_h)
{
	if(flip_h)
		obj->attr->attr1 |= ATTR1_HFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_HFLIP;
}

// set vertical flip value for a GameObj
void gameobj_set_flip_v(GameObj *obj, bool flip_v)
{
	if(flip_v)
		obj->attr->attr1 |= ATTR1_VFLIP;
	else
		obj->attr->attr1 &= ~ATTR1_VFLIP;
}




// push changes to a GameObj's position, animation, and palette
void gameobj_push_changes(GameObj *obj)
{
	u32 tile_id = obj->tile_id;
	if(obj->anim != NULL)
		tile_id += (obj->anim->tile_offset * obj->anim->cur_frame);

	obj->attr->attr2 = ATTR2_BUILD(
		tile_id,
		obj->pal_bank_id,
		obj->layer_priority
	);
	gameobj_update_pos(obj);
}




// hide all GameObjs
void gameobj_hide_all()
{
	obj_hide_multi(&objattr_buffer[0], free_obj);
}

// unhide all GameObjs
void gameobj_unhide_all()
{
	obj_unhide_multi(&objattr_buffer[0], DCNT_MODE0, free_obj);
}