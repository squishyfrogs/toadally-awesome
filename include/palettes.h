#ifndef PALETTES_H
#define PALETTES_H


#define PAL_ID_UI 				0x0
// player palette is 8 colors long, with grayscale variant in colors 9-16
#define PAL_ID_PLAYER 			0x1
// pals 4-7 repeat 3x, one base, one for grayscale and one for time-immune 
#define PAL_ID_OBJS				0x2
#define PAL_ID_OBJS_GRAY
// 16th palette is dedicated to color cycling
#define PAL_ID_GREENCYCLE		0xF

#define PAL_COUNT				16

void load_level_select_palettes();
void load_main_game_palettes();

int palette_load(const unsigned short *pal_data, int pal_slot_id);
int palette_load_simple(const unsigned short *pal_data);
void palette_free(int pal_slot_id);


void palette_activate_grayscale_mode();
void palette_deactivate_grayscale_mode();


void color_cycle_init();
void color_cycle_update();

#endif //PALETTES_H