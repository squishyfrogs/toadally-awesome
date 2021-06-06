#ifndef INPUT_H
#define INPUT_H

#include <tonc.h>

bool input_locked();
void input_unlock_override_all();

void input_lock_player();
void input_unlock_player();
void input_lock_ui();
void input_unlock_ui();


#endif //INPUT_H