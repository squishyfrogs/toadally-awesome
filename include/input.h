#ifndef INPUT_H
#define INPUT_H

#include <tonc.h>

typedef enum LockFlag_T {
	INPLCK_SYS		= 0x01,
	INPLCK_UI		= 0x02,
	INPLCK_PLAYER	= 0x04,
	INPLCK_TONGUE	= 0x08,
	INPLCK_PAUSE	= 0x10
} LockFlag;

bool input_locked();
u8 input_current_lock();
void input_unlock_override_all();

void input_lock(u8 lock_flag);
void input_unlock(u8 lock_flag);

#define KEY_NONDIR KEY_FIRE|KEY_SHOULDER|KEY_SPECIAL


#endif //INPUT_H