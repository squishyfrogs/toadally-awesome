#ifndef GAMEDATA_H
#define GAMEDATA_H

void gamedata_init();
void gamedata_save_byte(unsigned char data, int mem_offset);
unsigned char gamedata_load_byte(int mem_offset);

#endif //GAMEDATA_H