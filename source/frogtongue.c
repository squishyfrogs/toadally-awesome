#include "frogtongue.h"
#include "direction.h"
#include "map.h"
#include "sprites/tongue.h"

#define TSPR_PIECE_H	0
#define TSPR_PIECE_V	1
#define TSPR_TIP_H		2
#define TSPR_TIP_V		3

#define TONGUE_EXT_TL	2	// how many tiles forward the tongue can reach
#define TONGUE_EXT_PX	40	// max reach of tongue
#define EXT_SPD			2

int tongue_maximum;			// max px length of tongue extension (accounting for objects)
int tongue_extension;		// how far (in pixels) the tongue is currently extended
bool tongue_out;			// is tongue out of mouth?
int extend_speed = EXT_SPD;

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


void tongue_update()
{
	Vector2 t = tongue_owner->tile_pos;
	Vector2 p = tongue_owner->pixel_pos;
	Vector2 dir = dir_to_vec(gameobj_get_facing(tongue_owner));

	if(tongue_out)
	{
		tongue_extension += extend_speed;
		if(tongue_extension >= tongue_maximum)
		{
			tongue_extension = tongue_maximum;
			//tongue_retract();
		}
		if(tongue_extension <= 0)
		{
			extend_speed *= -1;
			tongue_store();
		}
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
	if(tongue_out)
		return;

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
	tongue_maximum = 8 + (16 * tmax);
	tongue_out = true;
	tongue_extension = 0;
	extend_speed = EXT_SPD;

	

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
}

void tongue_retract()
{
	extend_speed = -EXT_SPD;
}

void tongue_store()
{
	tongue_maximum = TONGUE_EXT_PX;
	tongue_extension = 0;
	tongue_out = false;

	for(int i = 0; i < TONGUE_PIECES; i++)
		gameobj_hide(tongue_pieces[i]);
	gameobj_hide(tongue_tip);
}


void tongue_attach_obj(GameObj *target)
{
	attached_obj = target;
}

GameObj *tongue_get_attached_object()
{
	return attached_obj;
}

bool check_tongue_out()
{
	return tongue_out;
}