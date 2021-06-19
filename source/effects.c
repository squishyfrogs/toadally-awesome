#include "effects.h"
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

GameObj *eff_templates[ET_COUNT];		// hidden templates for the different effects
#define EFFECTS_MAX 8
Effect effects[EFFECTS_MAX];			// effects that will be visible in game

#define DUST_TIMER_MAX 5

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
	eff_templates[ET_DUST] = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, dust_tile, 0, 0, 0);
	gameobj_set_sprite_offset(eff_templates[ET_DUST], 0, 0);
	gameobj_hide(eff_templates[ET_DUST]);

	// init smoke_cloud
	int smoke_tile = mem_load_tiles(smokeTiles, smokeTilesLen);
	eff_templates[ET_SMOKE] = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, smoke_tile, 0, 0, 0);
	gameobj_set_sprite_offset(eff_templates[ET_SMOKE], 0, 0);
	AnimationData *smoke_data = animdata_create(smoke_tile, ANIM_OFFSET_16x16, 4, 0);
	gameobj_set_anim_data(eff_templates[ET_SMOKE], smoke_data, 0);
	gameobj_hide(eff_templates[ET_SMOKE]);
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




void create_effect_at_tile(EffectType eff_type, int tile_id)
{
	Effect *eff = get_free_effect();
	// if no free effect slots, dont bother
	if(eff == NULL)
		return;
	gameobj_clone(eff->obj, eff_templates[eff_type]);
	gameobj_set_tile_pos_by_id(eff->obj, tile_id);
	gameobj_unhide(eff->obj);
	gameobj_play_anim(eff->obj);
	eff->life_timer = SMOKE_TIMER_MAX;
}

void create_effect_at_position(EffectType eff_type, int tile_x, int tile_y)
{
	Effect *eff = get_free_effect();
	// if no free effect slots, dont bother
	if(eff == NULL)
		return;
	gameobj_clone(eff->obj, eff_templates[eff_type]);
	gameobj_set_tile_pos(eff->obj, tile_x, tile_y);
	gameobj_unhide(eff->obj);
	gameobj_play_anim(eff->obj);
	eff->life_timer = SMOKE_TIMER_MAX;
}