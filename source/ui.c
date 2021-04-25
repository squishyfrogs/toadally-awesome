#include <tonc.h>
#include "gameobj.h"
#include "layers.h"
#include "sprites/ui/heart.h"

void ui_init();
void ui_update();

#define HP_MAX 6
GameObj *hearts[HP_MAX];	//health bar 

int player_health = 3;
int h_pal;
int h_tile;

void ui_init()
{
	h_pal = mem_load_palette(heartPal);
	h_tile = mem_load_tiles(heartTiles, heartTilesLen);
	for(int i = 0; i < HP_MAX; i++)
	{
		hearts[i] = init_gameobj();
		hearts[i]->priority = LAYER_OVERLAY;
		gameobj_update_attr_full(hearts[i], ATTR0_TALL, ATTR1_SIZE_8x16, h_pal, h_tile, 16+i*8, 0, true);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}
}

void ui_update()
{
	for(int i = 0; i < HP_MAX; i++)
	{
		if(i >= player_health)
			hearts[i]->tile_id = h_tile + 2;
		else
			hearts[i]->tile_id = h_tile;
		gameobj_update_attr(hearts[i]);
		if(i % 2)
			gameobj_set_flip_h(hearts[i], true);
	}
}