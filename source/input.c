#include "input.h"


///////////////////
/// Input Locks ///
///////////////////

static u8 inp_lck = 0;

bool input_locked()
{
	return (inp_lck > 0);
}

u8 input_current_lock()
{
	return inp_lck;
}


void input_unlock_override_all()
{
	inp_lck = 0;
}

///////////////////////////////

void input_lock(u8 lock_flag)
{
	inp_lck |= lock_flag;
}

void input_unlock(u8 lock_flag)
{
	inp_lck &= ~lock_flag;
}
