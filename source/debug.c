#include <string.h>
#include <tonc.h>
#include "debug.h"


#define DEBUG_X 200
#define DEBUG_Y 10

static char debug_text[20];

void debug_write_text(char *str)
{
	//tte_erase_screen();
	tte_set_pos(DEBUG_X, DEBUG_Y);
	tte_erase_line();
	tte_write(str);

}

void debug_write_int(int num)
{
	//tte_erase_screen();
	tte_set_pos(DEBUG_X, DEBUG_Y);
	tte_erase_line();
	sprintf(debug_text, "#{cx:1}%d", num);
	tte_write(debug_text);
}