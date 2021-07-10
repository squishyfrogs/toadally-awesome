#ifndef PLAYERHEALTH_H
#define PLAYERHEALTH_H

void playerhealth_init();

void playerhealth_reset();

int playerhealth_get();

void playerhealth_take_damage();

void playerhealth_damage_check();
void playerhealth_death_check();

bool playerhealth_is_dead();

void playerhealth_die();
void playerhealth_heal(int heal_amt);
void playerhealth_reduce_hp(int dmg_amt);


#define PLAYER_HP_MAX 6

#endif //PLAYERHEALTH_H


