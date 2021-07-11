#include <tonc.h>
#include "gamedata.h"
#include "debug.h"

void gamedata_init()
{
	if(DEBUG_CLEAR_SRAM)
	{
		toncset(sram_mem, 0, SRAM_SIZE);
	}
}

///////////////////
/// Save & Load ///
///////////////////

// 0x0E00_0000 to 0x0FFF_FFFF
// byte r/w only


void gamedata_save_byte(unsigned char data, int mem_offset)
{
	toncset(sram_mem + mem_offset, data, 1);
	
}



unsigned char gamedata_load_byte(int mem_offset)
{
	return (unsigned char)sram_mem[mem_offset];
	
}
