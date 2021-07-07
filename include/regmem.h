#ifndef REGMEM_H
#define REGMEM_H


int mem_load_palette(const unsigned short *pal_data, int pal_slot_id);
void mem_free_palette(int pal_id);
int mem_load_tiles(const unsigned short *tile_data, int data_len);
//void mem_free_tiles(int data_len);
void mem_clear_obj_palettes();
void mem_clear_tiles();

void reg_set_main();
void reg_set_title();
void reg_set_lev_sel();

void reg_save_game_data();
void reg_load_game_data();


#endif //REGMEM_H