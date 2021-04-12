
//{{BLOCK(gear)

//======================================================================
//
//	gear, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 5 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 4x4 
//	Total size: 32 + 160 + 32 = 224
//
//	Time-stamp: 2021-04-11, 22:45:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned short gearTiles[80] __attribute__((aligned(4)))=
{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x1000,0x0000,0x2100,0x0000,0x2210,0x0000,0x2221,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x1100,0x0000,0x2210,
	0x0001,0x2210,0x1012,0x2211,0x1122,0x2111,0x1122,0x1111,
	0x0000,0x2221,0x0000,0x2210,0x0000,0x1100,0x0000,0x1110,
	0x0000,0x1110,0x1000,0x1111,0x2100,0x1122,0x2100,0x1222,

	0x1122,0x0011,0x1112,0x0000,0x0111,0x0000,0x0011,0x0000,
	0x0001,0x0000,0x0001,0x0000,0x0000,0x0000,0x0000,0x0000,
};

const unsigned short gearMap[16] __attribute__((aligned(4)))=
{
	0x0001,0x0002,0x0402,0x0401,0x0003,0x0004,0x0404,0x0403,
	0x0803,0x0804,0x0C04,0x0C03,0x0801,0x0802,0x0C02,0x0C01,
};

const unsigned short gearPal[16] __attribute__((aligned(4)))=
{
	0x0000,0x3DCE,0x62D7,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//}}BLOCK(gear)