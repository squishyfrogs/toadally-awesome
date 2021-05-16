#include "direction.h"
#include"vector2.h"

Vector2 dir_to_vec(int direction)
{
	Vector2 v;
	switch(direction)
	{
		case DIRECTION_NORTH:
			v.x = 0;
			v.y = -1;
			break;
		case DIRECTION_SOUTH:
			v.x = 0;
			v.y = 1;
			break;
		case DIRECTION_EAST:
			v.x = 1;
			v.y = 0;
			break;
		case DIRECTION_WEST:
			v.x = -1;
			v.y = 0;
			break;
		default:
			v.x = 0;
			v.y = 0;
			break;
	}
	return v;
}

int vec_to_dir(Vector2 v)
{
	if(v.y < 0)
		return DIRECTION_NORTH;
	else if(v.y > 0)
		return DIRECTION_SOUTH;
	if(v.x < 0)
		return DIRECTION_WEST;
	else if(v.x > 0)
		return DIRECTION_EAST;
	else
		return 0;
}

int ints_to_dir(int x, int y)
{
	if(y < 0)
		return DIRECTION_NORTH;
	else if(y > 0)
		return DIRECTION_SOUTH;
	if(x < 0)
		return DIRECTION_WEST;
	else if(x > 0)
		return DIRECTION_EAST;
	else
		return 0;
}