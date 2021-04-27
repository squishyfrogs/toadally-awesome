#include <string.h>
#include <tonc.h>
#include "game.h"
#include "gameobj.h"
#include "layers.h"

void gameobj_init_all();
void gameobj_update_anim_all();
void gameobj_push_all_updates();

void gameobj_update_pos(GameObj *obj);


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

}



////////////////////////
/// UPDATE FUNCTIONS ///
////////////////////////


// update all GameObj animations (once per (vsync / anim_speed))
void gameobj_update_anim_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_update_anim(&obj_list[i]);
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
	obj->tile_id = 0;
	obj->pal_bank_id = 0;
	obj->priority = LAYER_GAMEOBJ;
	obj->spr_shape = ATTR0_SQUARE;
	obj->spr_size = ATTR1_SIZE_16x16;	// assume 16x6 for default bc I really doubt anyone is making 16x8 objects 

	obj->pos_x = 0;
	obj->pos_y = 0;
	obj->fixed_pos = false;

	obj->anim_tile_offset = 4;			// matches 16x16 sprite size
	obj->anim_frame_ct = 1;
	obj->anim_cur_frame = 0;


	return obj;
}



// set a GameObj's attributes
void gameobj_update_attr(GameObj *obj)
{
	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->tile_id));
	gameobj_update_pos(obj);
}

// set a GameObj's attributes
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y, bool fixed_pos)
{
	obj->spr_shape = attr0_shape;
	obj->spr_size = attr1_size;
	obj->pal_bank_id = palbank;
	obj->tile_id = tile_id;
	obj->pos_x = x;
	obj->pos_y = y;
	obj->fixed_pos = fixed_pos;

	u32 tid = obj->tile_id + (obj->anim_tile_offset * obj->anim_cur_frame);

	obj_set_attr(obj->attr, attr0_shape, attr1_size, (ATTR2_PALBANK(palbank) | tid));
	gameobj_update_pos(obj);
}


// set a GameObj's animation info
void gameobj_set_anim_info(GameObj *obj, u16 frame_count, short tile_offset)
{
	obj->anim_frame_ct = frame_count;
	obj->anim_tile_offset = tile_offset;
	obj->anim_cur_frame = 0;				//if we changed the anim info, its a safe bet that we want to reset its frame too 
}


// set a GameObj's position to new values
void gameobj_set_pos(GameObj *obj, int x, int y)
{
	obj->pos_x = x;
	obj->pos_y = y;
	gameobj_update_pos(obj);
}

// translate a GameObj by (x,y)
void gameobj_change_pos(GameObj *obj, int move_x, int move_y)
{
	obj->pos_x += move_x;
	obj->pos_y += move_y;
	gameobj_update_pos(obj);
}

// update a GameObj's attrs based on its current position
void gameobj_update_pos(GameObj *obj)
{
	if(obj->fixed_pos)
		obj_set_pos(obj->attr, obj->pos_x, obj->pos_y);
	else
		obj_set_pos(obj->attr, obj->pos_x - world_offset_x, obj->pos_y - 2 - world_offset_y);	//subtract 2 to center sprites on tiles better
}

//advance a GameObj's animation one frame
void gameobj_update_anim(GameObj* obj)
{
	if(obj->anim_frame_ct > 0)
	{
		obj->anim_cur_frame = (obj->anim_cur_frame + 1) % obj->anim_frame_ct;		
	}
}


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







// push changes to a GameObj's position, animation, and palette
void gameobj_push_changes(GameObj *obj)
{
	u32 tile_id = obj->tile_id + (obj->anim_tile_offset * obj->anim_cur_frame);
	obj->attr->attr2 = ATTR2_BUILD(
		tile_id,
		obj->pal_bank_id,
		obj->priority
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