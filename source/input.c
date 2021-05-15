#include "input.h"


static bool inp_lck = false;

void input_lock()
{
	inp_lck = true;
}

void input_unlock()
{
	inp_lck = false;
}

bool input_locked()
{
	return inp_lck;
}