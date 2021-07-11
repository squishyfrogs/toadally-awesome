#include <tonc.h>
#include "gamedata.h"
#include "debug.h"

// (basically my answer to not knowing what SRAM first initializes with, but also can ensure data compatibility with version changes)
#define SRAM_KEY	0xAD		// "unique" key || if first byte of SRAM is not set to this, erase everything 

void clear_all_sram();



void gamedata_init()
{
	byte key = gamedata_load_byte(0);
	if(DEBUG_CLEAR_SRAM || (key != SRAM_KEY))
	{
		clear_all_sram();
	}
	gamedata_save_byte(SRAM_KEY, 0);
}

void clear_all_sram()
{
	for(int i = 0; i < SRAM_SIZE; i++)
	{
		sram_mem[i] = 0;
	}
}

///////////////////
/// Save & Load ///
///////////////////

// 0x0E00_0000 to 0x0FFF_FFFF
// byte r/w only


void gamedata_save_byte(byte data, int mem_offset)
{ 
	sram_mem[mem_offset] = data;
}

byte gamedata_load_byte(int mem_offset)
{
	return (byte)sram_mem[mem_offset];
}
