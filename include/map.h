#ifndef MAP_H
#define MAP_H


void load_map_palette(const unsigned short *map_palette);
void load_map_tiles(const unsigned short *map_tiles, int tiles_len);
void load_map(const unsigned short *map, int map_len);
void load_map_col_info(const unsigned short *map_col);

ushort get_tile_col_info(int pos_x, int pos_y);

#endif //MAP_H