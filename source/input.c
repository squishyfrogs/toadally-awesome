#include "input.h"


static bool input_lock = false;

void lock_input()
{
	input_lock = 1;
}

void unlock_input()
{
	input_lock = 0;
}

bool input_locked()
{
	return input_lock;
}