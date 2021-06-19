#include "input.h"


///////////////////
/// Input Locks ///
///////////////////

#define INPLCK_PLAYER	0x01
#define INPLCK_UI		0x02
#define INPLCK_SYS		0x04

static u8 inp_lck = 0;


bool input_locked()
{
	return (inp_lck > 0);
}


void input_unlock_override_all()
{
	inp_lck = 0;
}


void input_lock_player()
{
	inp_lck |= INPLCK_PLAYER;
}

void input_unlock_player()
{
	inp_lck &= ~INPLCK_PLAYER;
}


void input_lock_ui()
{
	inp_lck |= INPLCK_UI;
}

void input_unlock_ui()
{
	inp_lck &= ~INPLCK_UI;
}

void input_lock_sys()
{
	inp_lck |= INPLCK_SYS;
}

void input_unlock_sys()
{
	inp_lck &= ~INPLCK_SYS;
}
