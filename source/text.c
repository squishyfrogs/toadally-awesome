#include "text.h"
#include <tonc.h>
#include <string.h>



void textsys_init()
{
// --- (1) Base TTE init for tilemaps ---
	tte_init_se(
		0,						// Background number (BG 0)
		BG_CBB(0)|BG_SBB(31),	// BG control (for REG_BGxCNT)
		1,						// Tile offset (special cattr)
		CLR_BLACK,				// Ink color
		15,						// BitUnpack offset (on-pixel = 15)
		NULL,					// Default font (sys8)
		NULL);					// Default renderer (se_drawg_s)
//	tte_init_chr4c
	
}


// libtonc TTE init but without the manual color override
void tte_init_se_no_color(int bgnr, u16 bgcnt, SCR_ENTRY se0, u32 bupofs, 
	const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &fwf_default;
	if(proc==NULL)	proc= se_drawg_default;

	tte_init_base(font, proc, se_erase);

	TTC *tc= tte_get_context();
	TSurface *srf= &tc->dst;

	srf_init(srf, SRF_BMP16, se_mem[BFN_GET(bgcnt, BG_SBB)],
		32, 32, 16, pal_bg_mem);
	srf_set_pal(srf, pal_bg_mem, 256);

	tc->flags0= bgnr;
	tc->ctrl= bgcnt;
	REG_BGCNT[bgnr]= bgcnt;

	// --- Prep color attributes and palette ---
	uint dstB= (bgcnt & BG_8BPP) ? 8 : 4;
	uint ink, shadow=0, paper;
	uint se= BFN_GET(se0, SE_ID);

	if(dstB == 4)
	{
		u32 pb= BFN_GET(se0, SE_PALBANK)*16;

		ink= pb + ((bupofs+1)&15);
		if( (bupofs&~BUP_ALL_OFS) > 15)
			shadow= pb + ((bupofs>>4)&15);
	}
	else
	{
		se *= 2;	// Correct for tile stride

		ink= (bupofs+1)&255;
		if( (bupofs&~BUP_ALL_OFS) > 255)
			shadow= (bupofs>>8)&255;
	}
	// Set paper entry (PONDER: color too somehow?)
	paper= (bupofs&BUP_ALL_OFS) ? bupofs : 0;
	
	tc->cattr[TTE_INK]= ink;
	tc->cattr[TTE_SHADOW]= shadow;
	tc->cattr[TTE_PAPER]= paper;
	tc->cattr[TTE_SPECIAL]= se0;

	//srf->palData[ink]= clrs&0xFFFF;
	//if(shadow)
	//	srf->palData[shadow]= clrs>>16;

	// --- Bitunpack font ---
	void *dstD= &tile_mem[BFN_GET(bgcnt, BG_CBB)][se];
	u32 dstS= font->charCount*font->cellSize;

	BUP bup= { dstS, font->bpp, dstB, bupofs };
	BitUnPack(font->data, dstD, &bup);
}