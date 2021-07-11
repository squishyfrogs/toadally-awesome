#include <tonc.h>
#include "regmem.h"
#include "palettes.h"
#include "gameobj.h"
#include "audio.h"
#include "sprites/ui/levSelCursor.h"
#include "sprites/ui/levelLock.h"
#include "map.h"
// main.c
extern void go_to_title();
extern void go_to_main_game();
// gameobj.c
extern void gameobj_push_changes(GameObj *obj);
extern void gameobj_update_all();
extern void gameobj_push_all_updates();

extern void init_objs_temp();

void level_select_init();
void level_select_update();
void level_select_clear();

void load_level_select_ui();
void unload_level_select_ui();
void reset_level_cursor();
void move_level_cursor(int x, int y);
void update_cursor_position();

void set_plaque_state(int plaque_id, int p_state);


#define LEVELS_PER_ROW		4
#define ROWS_PER_PAGE		4
#define LEVELS_PER_PAGE		(LEVELS_PER_ROW*ROWS_PER_PAGE)

#define LEV_SEL_PADDING_X 32
#define LEV_SEL_PADDING_Y 32
#define LEV_SEL_OFFSET_X 48
#define LEV_SEL_OFFSET_Y 32

static int cur_lvl_selection = 0;
GameObj *cursor;
static int plaque_tile;
GameObj *level_plaques[LEVELS_PER_PAGE];
#define PLAQUE_OFF_LOCK			1
#define PLAQUE_OFF_STAR_HOLE	2
#define PLAQUE_OFF_STAR			3


void level_select_init()
{
	load_level_select_ui();
	reset_level_cursor();
	//audio_play_track(MOD_LEV_SEL_2);
	audio_stop_track();
	//audio_play_track(MOD_LEVEL_SEL);
}

// update level select screen
void level_select_update()
{
	if(key_hit(KEY_CANCEL))
	{
		go_to_title();
	}
	if(key_hit(KEY_ACCEPT))
	{
		if(check_level_unlocked(cur_lvl_selection))
		{
			audio_play_sound(SFX_BLIP_HIGH);
			set_level_data(cur_lvl_selection+1);
			go_to_main_game();
		}
	}

	if(key_hit(KEY_DIR))
	{
		audio_play_sound(SFX_BLIP_SOFT);
		move_level_cursor(key_tri_horz(), key_tri_vert());
	}

	gameobj_update_all();
	gameobj_push_all_updates();
}

void level_select_clear()
{
	unload_level_select_ui();
}

////////////////////
/// Level Cursor ///
////////////////////

void load_level_select_ui()
{
	int ui_pal = mem_load_palette(levSelCursorPal, PAL_ID_UI);
	int cursor_tile = mem_load_tiles(levSelCursorTiles, levSelCursorTilesLen);
	cursor = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_WIDE, ATTR1_SIZE_64x32, ui_pal, cursor_tile, 32, 32, OBJPROP_FIXED_POS);
	gameobj_set_sprite_offset(cursor, 16,8);
	
	int plaque_pal = mem_load_palette(levelLockPal, 1);
	plaque_tile = mem_load_tiles(levelLockTiles, levelLockTilesLen);
	for(int i = 0; i < LEVELS_PER_PAGE; i++)
	{
		int col = i % LEVELS_PER_ROW;
		int row = i / LEVELS_PER_ROW;

		int x = (col * LEV_SEL_OFFSET_X) + LEV_SEL_PADDING_X + 16;
		int y = (row * LEV_SEL_OFFSET_Y) + LEV_SEL_PADDING_Y;
		level_plaques[i] = gameobj_init_full(LAYER_GAMEOBJ, ATTR0_SQUARE, ATTR1_SIZE_16x16, plaque_pal, plaque_tile, x, y, OBJPROP_FIXED_POS);
		
		if(check_level_cleared(i))
			set_plaque_state(i, PLAQUE_OFF_STAR);
		else if(check_level_unlocked(i))
			set_plaque_state(i, PLAQUE_OFF_STAR_HOLE);
		else	
			set_plaque_state(i, PLAQUE_OFF_LOCK);
	}

}

void unload_level_select_ui()
{
	mem_free_palette(cursor->pal_bank_id);
	gameobj_erase(cursor);
}

// return cursor to level 1
void reset_level_cursor()
{
	cur_lvl_selection = 0;
	update_cursor_position();
}

// 
void move_level_cursor(int x, int y)
{
	int col = cur_lvl_selection % LEVELS_PER_ROW;
	int row = cur_lvl_selection / LEVELS_PER_ROW;

	col = (col + x) % LEVELS_PER_ROW;
	row = (row + y) % ROWS_PER_PAGE;

	cur_lvl_selection = (row * LEVELS_PER_ROW) + col;

	cur_lvl_selection = (cur_lvl_selection + LEVELS_PER_PAGE) % LEVELS_PER_PAGE;

	update_cursor_position();
}


void update_cursor_position()
{
	int col = cur_lvl_selection % LEVELS_PER_ROW;
	int row = cur_lvl_selection / LEVELS_PER_ROW;

	int x = (col * LEV_SEL_OFFSET_X) + LEV_SEL_PADDING_X;
	int y = (row * LEV_SEL_OFFSET_Y) + LEV_SEL_PADDING_Y;
	gameobj_set_pixel_pos(cursor, x, y);
	gameobj_push_changes(cursor);
}

void set_plaque_state(int plaque_id, int p_state)
{
	p_state = clamp(p_state, 0, 4);
	level_plaques[plaque_id]->spr_tile_id = plaque_tile + (4*p_state);
}