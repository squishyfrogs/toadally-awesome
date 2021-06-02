#include "game.h"
#include "gameobj.h"
#include "sprites/effects/dustcloud.h"
#include "sprites/effects/smoke.h"


void effects_init();
void effects_anim_update();
void create_effect_at_position(int tile_x, int tile_y);
void create_smoke_at_tile(int tile_id);



GameObj *dust_cloud;
static int dust_timer;
#define DUST_TIMER_MAX 5

GameObj *smoke_cloud;
static int smoke_timer;
#define SMOKE_TIMER_MAX 5

void effects_init()
{
	int effects_pal = mem_load_palette(dustcloudPal);

	// dust
	int dust_tile = mem_load_tiles(dustcloudTiles, dustcloudTilesLen);
	dust_cloud = init_gameobj_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, dust_tile, 0, 0, 0);
	gameobj_set_sprite_offset(dust_cloud, 0, 0);
	gameobj_hide(dust_cloud);

	// smoke_cloud
	int smoke_tile = mem_load_tiles(smokeTiles, smokeTilesLen);
	smoke_cloud = init_gameobj_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, effects_pal, smoke_tile, 0, 0, 0);
	gameobj_set_sprite_offset(smoke_cloud, 0, 0);
	smoke_cloud->anim = anim_create(smoke_tile, ANIM_OFFSET_16x16, 4, 0, 0);
	gameobj_hide(smoke_cloud);
}


void effects_anim_update()
{
	if(dust_timer > 0)
	{
		dust_timer--;
		if(dust_timer <= 0)
			gameobj_hide(dust_cloud);
	}

	if(smoke_timer > 0)
	{
		smoke_timer--;
		if(smoke_timer <= 0)
			gameobj_hide(smoke_cloud);
	}
}


void create_effect_at_position(int tile_x, int tile_y)
{
	gameobj_set_tile_pos(dust_cloud, tile_x, tile_y);
	gameobj_unhide(dust_cloud);
	dust_timer = DUST_TIMER_MAX;
}


void create_smoke_at_tile(int tile_id)
{
	gameobj_set_tile_pos_by_id(smoke_cloud, tile_id);
	gameobj_unhide(smoke_cloud);
	anim_play(smoke_cloud->anim);
	smoke_timer = SMOKE_TIMER_MAX;
}