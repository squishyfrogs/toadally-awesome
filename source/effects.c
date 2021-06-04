#include "game.h"
#include "gameobj.h"
#include "sprites/effects/dustcloud.h"
#include "sprites/effects/smoke.h"

typedef struct struct_Effect {
	bool in_use;						// is this effect currently in use?
	int life_timer;						// how long the effect will last before fading, in anim_frames
	GameObj *obj;						// effect GameObj data
}Effect;

void effects_init();
void effects_anim_update();
Effect *get_free_effect();
void create_effect_at_position(int tile_x, int tile_y);
void create_smoke_at_tile(int tile_id);

#define EFFECTS_MAX 8
Effect effects[EFFECTS_MAX];			// effects that will be visible in game

GameObj *dust_cloud;					// hidden template for dust cloud
#define DUST_TIMER_MAX 5

GameObj *smoke_cloud_src;				// hidden template for smoke cloud
#define SMOKE_TIMER_MAX 5

void effects_init()
{
	// init free effects
	for(int i = 0; i < EFFECTS_MAX; i++)
	{
		effects[i].in_use = false;
		effects[i].life_timer = 0;
		effects[i].obj = gameobj_init();
	}

	int effects_pal = mem_load_palette(dustcloudPal);

	// dust
	int dust_tile = mem_load_tiles(dustcloudTiles, dustcloudTilesLen);
	dust_cloud = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, dust_tile, 0, 0, 0);
	gameobj_set_sprite_offset(dust_cloud, 0, 0);
	gameobj_hide(dust_cloud);

	// init smoke_cloud
	int smoke_tile = mem_load_tiles(smokeTiles, smokeTilesLen);
	smoke_cloud_src = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, smoke_tile, 0, 0, 0);
	gameobj_set_sprite_offset(smoke_cloud_src, 0, 0);
	AnimationData *smoke_data = animdata_create(smoke_tile, ANIM_OFFSET_16x16, 4, 0);
	gameobj_set_anim_data(smoke_cloud_src, smoke_data, 0);
	gameobj_hide(smoke_cloud_src);
}


void effects_anim_update()
{
	for(int i = 0; i < EFFECTS_MAX; i++)
	{
		if(effects[i].in_use && effects[i].life_timer > 0)
		{
			effects[i].life_timer--;
			if(effects[i].life_timer <= 0)
			{
				gameobj_hide(effects[i].obj);
				effects[i].in_use = false;
			}
		}
	}
}

Effect *get_free_effect()
{
	for(int i = 0; i < EFFECTS_MAX; i++)
	{
		if(!effects[i].in_use)
		{
			effects[i].in_use = true;
			return &effects[i];
		}
	}
	return NULL;
}


void create_effect_at_position(int tile_x, int tile_y)
{
	
}


void create_smoke_at_tile(int tile_id)
{
	Effect *eff = get_free_effect();
	// if no free effect slots, dont bother
	if(eff == NULL)
		return;
	gameobj_clone(eff->obj, smoke_cloud_src);
	gameobj_set_tile_pos_by_id(eff->obj, tile_id);
	gameobj_unhide(eff->obj);
	gameobj_play_anim(eff->obj);
	eff->life_timer = SMOKE_TIMER_MAX;
}