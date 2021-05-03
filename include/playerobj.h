#ifndef PLAYEROBJ_H
#define PLAYEROBJ_H

#include "sprites/player.h"



#define PLAYER_MOVE_SPEED		1		// move speed in pixels per frame
#define PLAYER_TILE_OFFSET		4		// TILE offset for animations (how many tiles is one frame?)
#define PLAYER_FACING_OFFSET	8		// FRAME offset for each direction (multiply by tile offset for full offset)
#define PLAYER_ASYMMETRIC		0		// set 1 if player sheet has unique west-facing sprites

const int hop_arc[16] = {0, 2, 4, 5, 6, 7, 7, 8, 8, 7, 7, 6, 5, 4, 2, 0};

#endif