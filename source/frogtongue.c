#include "frogtongue.h"
#include "direction.h"
#include "sprites/tongue.h"

#define TSPR_STALK_H	0
#define TSPR_STALK_V	1
#define TSPR_TIP_H		2
#define TSPR_TIP_V		3

#define TONGUE_EXT_MAX	64

GameObj *tongue_tip;
int base_tile;				//tile_id of the first part of the tongue
GameObj *tongue_owner;
int tongue_extension;		// how far (in pixels) the tongue is currently extended

void tongue_init(GameObj *owner)
{
	int pal = owner->pal_bank_id;
	base_tile = mem_load_tiles(tongueTiles, tongueTilesLen) ;
	tongue_tip = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_8x8, pal, base_tile+TSPR_TIP_H, owner->tile_pos.x, owner->tile_pos.y, 0);
	vec2_set(&tongue_tip->spr_off, -4, 0);
	tongue_set_owner(owner);
	tongue_extension = 0;
}


void tongue_set_owner(GameObj *owner)
{
	tongue_owner = owner;
}


void tongue_update()
{
	Vector2 t = tongue_owner->tile_pos;
	Vector2 p = tongue_owner->pixel_pos;

	if(tongue_extension > 0)
	{
		Vector2 v = dir_to_vec(gameobj_get_facing(tongue_owner));
		p.x += (tongue_extension * v.x);
		p.y += (tongue_extension * v.y);
	}


	gameobj_set_tile_pos(tongue_tip, t.x, t.y);
	gameobj_set_pixel_pos(tongue_tip, p.x, p.y);
}


void tongue_extend()
{
	tongue_extension = 24;
}


void tongue_retract()
{
	tongue_extension = 0;
}


void tongue_attach_obj()
{

}