#ifndef REGMEM_H
#define REGMEM_H


int mem_load_palette(const unsigned short *pal_data);
void mem_free_palette(int pal_id);
int mem_load_tiles(const unsigned short *tile_data, int data_len);
void mem_clear_palettes();
void mem_clear_tiles();

void reg_init_main();
void reg_init_title();
void reg_init_lev_sel();


#endif //REGMEM_H