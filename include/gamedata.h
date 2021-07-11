#ifndef GAMEDATA_H
#define GAMEDATA_H

void gamedata_init();
void gamedata_save_byte(unsigned char data, int mem_offset);
unsigned char gamedata_load_byte(int mem_offset);

#define SRAM_OFFSET_LEVEL	0x0F

#endif //GAMEDATA_H