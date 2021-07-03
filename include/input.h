#ifndef INPUT_H
#define INPUT_H

#include <tonc.h>

bool input_locked();
int input_current_lock();
void input_unlock_override_all();

void input_lock_sys();
void input_lock_ui();
void input_unlock_ui();
void input_unlock_sys();
void input_lock_player();
void input_unlock_player();
void input_lock_tongue();
void input_unlock_tongue();

#define KEY_NONDIR KEY_FIRE|KEY_SHOULDER|KEY_SPECIAL


#endif //INPUT_H