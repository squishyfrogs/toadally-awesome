#include "frogtongue.h"
#include "input.h"
#include "direction.h"
#include "map.h"
#include "effects.h"
#include "sprites/tongue.h"

#define TSPR_PIECE_H	0
#define TSPR_PIECE_V	1
#define TSPR_TIP_H		2
#define TSPR_TIP_V		3

typedef enum TongueState_T{
	TS_STORED,			// tongue in mouth
	TS_EXTENDING,		// tongue extending from mouth
	TS_ATTACHED,		// tongue extended and attached to an object
	TS_RETRACTING,		// tongue returning to mouth
	TS_PULLING_OBJ,		// tongue returning to mouth, but dragging an attached obj with it
	TS_PULLING_PL		// tongue returning to mouth, but dragging the player toward the attached obj
} TongueState;

#define TONGUE_EXT_TL	2	// how many tiles forward the tongue can reach
//#define TONGUE_EXT_PX	((TONGUE_EXT_TL*16)+8)		// max reach of tongue
#define EXT_SPD			2	// how many pixels per frame the tongue extends/retracts

// objhistory.c
extern void history_update_all();
// camera.c
extern void camera_update_pos();

void tongue_check();						// check for objects at the apex of tongue stretching
void tongue_attach_obj(GameObj *target);	// attach the tongue to a valid target
void tongue_update_length();				// handles extending/retracting


int tongue_max_tl;		// max tile length of tongue extension (accounting for objects)
inline int tongue_len_px(int tiles){ return ((tiles*16)+8); }	// converts tile length to pixel length for tongue

int tongue_extension;		// how far (in pixels) the tongue is currently extended
TongueState tongue_state;

GameObj *tongue_tip;
int base_tile;				// tile_id of the tongue sprite sheet
#define TONGUE_PIECES 6
GameObj *tongue_pieces[TONGUE_PIECES];	// the "neck" of the tongue

GameObj *tongue_owner;		// playerobj, essentially
GameObj *attached_obj;		// what the tongue is stuck on, if anything


void tongue_init(GameObj *owner)
{
	Vector2 pos = owner->tile_pos;
	int pal = owner->pal_bank_id;
	base_tile = mem_load_tiles(tongueTiles, tongueTilesLen);
	tongue_tip = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_8x8, pal, base_tile+TSPR_TIP_H, pos.x, pos.y, 0);
	vec2_set(&tongue_tip->spr_off, -4, 0);
	tongue_set_owner(owner);
	tongue_extension = 0;
	attached_obj = NULL;

	for(int i = 0; i < TONGUE_PIECES; i++)
	{
		tongue_pieces[i] = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_8x8, pal, base_tile+TSPR_PIECE_H, pos.x, pos.y, 0);
		vec2_set(&tongue_pieces[i]->spr_off, -4, 0);
	}
	tongue_store();
}


void tongue_set_owner(GameObj *owner)
{
	tongue_owner = owner;
}


// handles extending/retracting
void tongue_update()
{
	Vector2 t = tongue_owner->tile_pos;
	Vector2 p = tongue_owner->pixel_pos;
	Vector2 dir = dir_to_vec(gameobj_get_facing(tongue_owner));
	switch (tongue_state)
	{
	case TS_EXTENDING:
		tongue_extension += EXT_SPD;
		if(tongue_extension >= tongue_len_px(tongue_max_tl))
		{
			tongue_extension = tongue_len_px(tongue_max_tl);
			tongue_check();
		}
		break;
	case TS_RETRACTING:
		tongue_extension -= EXT_SPD;
		if(tongue_extension <= 0)
		{
			tongue_extension = 0;
			tongue_store();
		}
		break;
	case TS_PULLING_OBJ:
		if(tongue_extension > 8)
		{
			if(tongue_extension - EXT_SPD <= 8)
			{

				gameobj_change_pixel_pos(attached_obj, dir.x * (8 - tongue_extension), dir.y * (8 - tongue_extension));
				gameobj_update_current_tile(attached_obj);
				history_update_all();
			}
			else
				gameobj_change_pixel_pos(attached_obj, dir.x * -EXT_SPD, dir.y * -EXT_SPD);
		}
		tongue_extension -= EXT_SPD;
		if(tongue_extension <= 0)
		{
			tongue_extension = 0;
			tongue_store();
		}
		break;
	case TS_PULLING_PL:
		if(tongue_extension > 8)
		{
			if(tongue_extension - EXT_SPD <= 8)
			{

				gameobj_change_pixel_pos(tongue_owner, dir.x * (tongue_extension - 8), dir.y * (tongue_extension - 8));
				gameobj_update_current_tile(tongue_owner);
				history_update_all();
			}
			else
				gameobj_change_pixel_pos(tongue_owner, dir.x * EXT_SPD, dir.y * EXT_SPD);
		}
		tongue_extension -= EXT_SPD;
		if(tongue_extension <= 0)
		{
			tongue_extension = 0;
			tongue_store();
		}
		camera_update_pos();
		break;
	default:
		break;
	}
		
	

	if(tongue_extension > 0)
	{
		p.x += (tongue_extension * dir.x);
		p.y += (tongue_extension * dir.y);
	}

	gameobj_set_tile_pos(tongue_tip, t.x, t.y);
	gameobj_set_pixel_pos(tongue_tip, p.x, p.y);
	int increment = (tongue_extension + 2) / TONGUE_PIECES;
	
	for(int i = 0; i < TONGUE_PIECES; i++)
	{
		if(tongue_extension > 12)
		{
			gameobj_unhide(tongue_pieces[i]);
		}
		else{
			gameobj_hide(tongue_pieces[i]);
			continue;
		}
		Vector2 inc = tongue_owner->pixel_pos;
		int spr_off = 2;
		inc.x += (((increment * i) + spr_off) * dir.x);
		inc.y += (((increment * i) + spr_off) * dir.y);
		gameobj_set_tile_pos(tongue_pieces[i], t.x, t.y);
		gameobj_set_pixel_pos(tongue_pieces[i], inc.x, inc.y);
	}
}


void tongue_extend()
{
	if(tongue_state != TS_STORED)
		return;
	tongue_state = TS_EXTENDING;

	Vector2 dir = dir_to_vec(gameobj_get_facing(tongue_owner));
	int tmax = 0;
	for(tmax = 0; tmax < TONGUE_EXT_TL; tmax++)
	{
		Vector2 v = tongue_owner->tile_pos;
		v.x += (1 + tmax) * dir.x;
		v.y += (1 + tmax) * dir.y;
		if(!check_tile_free(v.x, v.y))
			break;
	}
	tongue_max_tl = tmax;
	tongue_extension = 0;

	

	if(dir.x != 0)
	{
		tongue_tip->spr_tile_id = base_tile + TSPR_TIP_H;
		for(int i = 0; i < TONGUE_PIECES; i++)
			tongue_pieces[i]->spr_tile_id = base_tile + TSPR_PIECE_H;
	}
	else if(dir.y != 0)
	{
		tongue_tip->spr_tile_id = base_tile + TSPR_TIP_V;
		for(int i = 0; i < TONGUE_PIECES; i++)
			tongue_pieces[i]->spr_tile_id = base_tile + TSPR_PIECE_V;
	}
	

	//for(int i = 0; i < TONGUE_PIECES; i++)
	//	gameobj_unhide(tongue_pieces[i]);
	gameobj_unhide(tongue_tip);
	input_lock_tongue();
}


void tongue_retract()
{
	if(attached_obj != NULL)
	{
		// pull the attached object
		create_effect_at_position(ET_SMOKE, attached_obj->tile_pos.x, attached_obj->tile_pos.y);
		tongue_state = TS_PULLING_PL;
		input_lock_tongue();
	}
	else
	{
		tongue_state = TS_RETRACTING;
		tongue_detach_obj();
		input_lock_tongue();
	}
}


void tongue_store()
{
	tongue_state = TS_STORED;
	tongue_max_tl = TONGUE_EXT_TL;
	tongue_extension = 0;
	attached_obj = NULL;

	for(int i = 0; i < TONGUE_PIECES; i++)
		gameobj_hide(tongue_pieces[i]);
	gameobj_hide(tongue_tip);

	input_unlock_tongue();
}


//checks if touching a valid object, and attaches if true or retracts if false
void tongue_check()
{
	Vector2 v = tongue_owner->tile_pos;
	Vector2 dir = dir_to_vec(gameobj_get_facing(tongue_owner));
	v.x += (1 + tongue_max_tl) * dir.x;
	v.y += (1 + tongue_max_tl) * dir.y;
	if(!check_tile_free(v.x, v.y))
	{
		GameObj *obj = get_tile_contents(v.x,v.y);
		if(obj != NULL)
		{
			tongue_attach_obj(obj);
			input_unlock_tongue();
			return;
		}
	}
	tongue_retract();
}

void tongue_attach_obj(GameObj *target)
{
		
	attached_obj = target;
	if(target != NULL)
		tongue_state = TS_ATTACHED;
}

void tongue_detach_obj()
{
	attached_obj = NULL;
}

GameObj *tongue_get_attached_object()
{
	return attached_obj;
}

bool check_tongue_out()
{
	return (tongue_state != TS_STORED);
}