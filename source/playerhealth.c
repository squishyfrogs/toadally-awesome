#include <tonc.h>
#include "playerhealth.h"
#include "objhistory.h"


static int player_health = PLAYER_HP_MAX;
static bool player_damaged;


void playerhealth_init()
{
	playerhealth_reset();
}

void playerhealth_reset()
{
	player_damaged = false;
	player_health = PLAYER_HP_MAX;
}

int playerhealth_get()
{
	return player_health;
}

// flag the player as damaged 
void playerhealth_take_damage()
{
	player_damaged = true;
}


void playerhealth_damage_check()
{
	if(player_damaged)
	{
		player_health--;
		// TODO: maybe change this to not rewind? 
		history_step_back(1);
	}
	player_damaged = false;
}

void playerhealth_death_check()
{
	if(player_health <= 0)
	{
		playerhealth_die();
	}
}

void playerhealth_die()
{

}


void playerhealth_heal(int heal_amt)
{


}