#ifndef MEMORY_H
#define MEMORY_H


int mem_load_palette(const unsigned short *pal_data);
int mem_load_tiles(const unsigned short *tile_data, int data_len);
void mem_clear_palettes();
void mem_clear_tiles();

void reg_init_main();
void reg_init_title();


#endif //MEMORY_H