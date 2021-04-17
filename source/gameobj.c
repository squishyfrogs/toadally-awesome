#include <string.h>
#include <tonc.h>
#include "gameobj.h"
#include "sprites.h"
// sprite includes 

#define SPRITE_CT 3

void init_objs();


GameObj *init_gameobj();
GameObj *init_gameobj_with_id(int obj_id);

void game_update();
void update_anim_all();
void update_gameobj_all();

int mem_load_palette(const ushort *pal_data);
int mem_load_tiles(const ushort *tile_data, int data_len);

void gameobj_update_attr(GameObj *obj);
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y);
void gameobj_set_anim_info(GameObj *obj, u16 frame_count, u16 tile_offset);
void gameobj_set_pos(GameObj *obj, int x, int y);
void gameobj_change_pos(GameObj *obj, int move_x, int move_y);
void gameobj_update_pos(GameObj *obj);
void gameobj_update_anim(GameObj *obj);
void gameobj_update(GameObj *obj);


GameObj obj_list[OBJ_COUNT];
OBJ_ATTR obj_buffer[OBJ_COUNT];
static int free_obj = 0;		//marker of first free obj in the object list
static int free_pal = 0;		//marker of first free palette in VRAM
static int free_tile = 0;		//marker of first free tile in VRAM



GameObj *kirby;

GameObj *gear;

GameObj *mario;

void init_objs()
{
	//memcpy(&tile_mem[4][0], metrTiles, metrTilesLen);
	//memcpy(pal_obj_mem, metrPal, metrPalLen);
	// Places the glyphs of a 4bpp boxed kirby sprite 
	// into LOW obj memory (cbb == 4)



//	mem_load_tiles(gearTiles, gearTilesLen);
//	memcpy(pal_obj_mem + 16, gearPal, gearPalLen);
//	mem_load_tiles(marioTiles, marioTilesLen);
//	memcpy(pal_obj_mem + 32, marioPal, marioPalLen);

	//hides all sprites
	oam_init(obj_buffer, 128);


	// init kirby
	kirby = init_gameobj();
	int k_pal = mem_load_palette(kirbyPal);
	int k_tile = mem_load_tiles(kirbyTiles, kirbyTilesLen);
	gameobj_update_attr_full(kirby, ATTR0_SQUARE, ATTR1_SIZE_32x32, k_pal, k_tile, 96, 96);

	// init gear
	gear = init_gameobj();
	k_pal = mem_load_palette(gearPal);
	k_tile = mem_load_tiles(gearTiles, gearTilesLen);
	gameobj_update_attr_full(gear, ATTR0_SQUARE, ATTR1_SIZE_32x32, 1, k_tile, 0, 128);
	
	// init mario
	mario = init_gameobj();
	k_pal = mem_load_palette(marioPal);
	k_tile = mem_load_tiles(marioTiles, marioTilesLen);
	gameobj_update_attr_full(mario, ATTR0_SQUARE, ATTR1_SIZE_32x32, 2, k_tile, 140, 80);
	gameobj_set_anim_info(mario, 4, 16);

}


void game_update()
{

	gameobj_change_pos(kirby, 2*key_tri_horz(), 2*key_tri_vert());

	// increment/decrement starting tile with R/L
	// tid += bit_tribool(key_hit(-1), KI_R, KI_L);

	if(key_hit(KEY_L))
	{
		//hide/unhide sprites
		static bool spr_hidden;
		if(spr_hidden)
			obj_unhide_multi(kirby->attr, DCNT_MODE0, SPRITE_CT);
		else
			obj_hide_multi(kirby->attr, SPRITE_CT);
		spr_hidden = !spr_hidden;
	}
	

	// flip
	if(key_hit(KEY_A))	// horizontally
		kirby->attr->attr1 ^= ATTR1_HFLIP;
	if(key_hit(KEY_B))	// vertically
		kirby->attr->attr1 ^= ATTR1_VFLIP;
	

	// make it glow (via palette swapping)
	//pb= key_is_down(KEY_SELECT) ? 1 : 0;

	// toggle mapping mode
	//if(key_hit(KEY_START))
	//	REG_DISPCNT ^= DCNT_OBJ_1D;

	//REG_BG1HOFS= kirby_x;
	//REG_BG1VOFS= kirby_y;
	update_gameobj_all();

	// copy all changes into oam memory
	oam_copy(oam_mem, obj_buffer, SPRITE_CT);	// only need to update one
}




////////////////////////
/// UPDATE FUNCTIONS ///
////////////////////////


// update all GameObj animations (once per (vsync / anim_speed))
void update_anim_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_update_anim(&obj_list[i]);
	}
}

// update all GameObjs (once per vsync)
void update_gameobj_all()
{
	for(int i = 0; i < OBJ_COUNT; i++)
	{
		gameobj_update(&obj_list[i]);
	}
}




////////////////////////
/// MEMORY FUNCTIONS ///
////////////////////////

int mem_load_palette(const ushort *pal_data)
{
	int pal_id = free_pal;
	memcpy(pal_obj_mem + (pal_id * 16), pal_data, 32);	// 16 colors per palette, 2 bytes per color
	free_pal++;
	return pal_id;
}

int mem_load_tiles(const ushort *tile_data, int data_len)
{
	//always block 4? 
	int tile_id = free_tile;
	memcpy(&tile_mem[4][tile_id], tile_data, data_len);
	free_tile += data_len / 32;
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
	obj->attr = &obj_buffer[obj_id];
	obj->tile_id = 0;
	obj->pal_bank_id = 0;
	obj->priority = 0;
	obj->spr_shape = ATTR0_SQUARE;
	obj->spr_size = ATTR1_SIZE_16x16;	// assume 16x6 for default bc I really doubt anyone is making 16x8 objects 

	obj->pos_x = 0;
	obj->pos_y = 0;

	obj->anim_tile_offset = 4;			// matches 16x16 sprite size
	obj->anim_frame_ct = 1;
	obj->anim_cur_frame = 0;


	return obj;
}



// set a GameObj's attributes
void gameobj_update_attr(GameObj *obj)
{
	obj_set_attr(obj->attr, obj->spr_shape, obj->spr_size, (ATTR2_PALBANK(obj->pal_bank_id) | obj->tile_id));
}

// set a GameObj's attributes
void gameobj_update_attr_full(GameObj *obj, u16 attr0_shape, u16 attr1_size, u16 palbank, u32 tile_id, int x, int y)
{
	obj->spr_shape = attr0_shape;
	obj->spr_size = attr1_size;
	obj->pal_bank_id = palbank;
	obj->tile_id = tile_id;
	obj->pos_x = x;
	obj->pos_y = y;

	u32 tid = obj->tile_id + (obj->anim_tile_offset * obj->anim_cur_frame);

	obj_set_attr(obj->attr, attr0_shape, attr1_size, (ATTR2_PALBANK(palbank) | tid));
	obj_set_pos(obj->attr, x, y);
}


// set a GameObj's animation info
void gameobj_set_anim_info(GameObj *obj, u16 frame_count, u16 tile_offset)
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
	obj_set_pos(obj->attr, obj->pos_x, obj->pos_y);
}

// translate a GameObj by (x,y)
void gameobj_change_pos(GameObj *obj, int move_x, int move_y)
{
	obj->pos_x += move_x;
	obj->pos_y += move_y;
	obj_set_pos(obj->attr, obj->pos_x, obj->pos_y);
}

// update a GameObj's attrs based on its current position
void gameobj_update_pos(GameObj *obj)
{
	obj_set_pos(obj->attr, obj->pos_x, obj->pos_y);
}

//advance a GameObj's animation one frame
void gameobj_update_anim(GameObj* obj)
{
	if(obj->anim_frame_ct > 0)
	{
		obj->anim_cur_frame = (obj->anim_cur_frame + 1) % obj->anim_frame_ct;		
	}
}

// update a GameObj's position, animation, and palette
void gameobj_update(GameObj *obj)
{
	u32 tile_id = obj->tile_id + (obj->anim_tile_offset * obj->anim_cur_frame);
	obj->attr->attr2 = ATTR2_BUILD(
		tile_id,
		obj->pal_bank_id,
		obj->priority
	);
	obj_set_pos(obj->attr, obj->pos_x, obj->pos_y);
}

