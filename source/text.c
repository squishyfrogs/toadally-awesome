#include "text.h"
#include <tonc.h>
#include <string.h>


void textsys_init()
{
// --- (1) Base TTE init for tilemaps ---
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		0,						// Tile offset (special cattr)
		CLR_WHITE,				// Ink color
		15,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8) 
		NULL);					// Default renderer (se_drawg_s)
	
}