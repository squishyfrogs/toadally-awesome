#ifndef DIRECTION_H
#define DIRECTION_H

// directions are used for sprite sheets, arrange sprites in the appropriate order
#define DIRECTION_SOUTH		0		// 00 || facing screen 
#define DIRECTION_NORTH		1		// 01 || facing away
#define DIRECTION_EAST		2		// 10 || facing right
#define DIRECTION_WEST		3		// 11 || facing left || optional, only needed for asymmetric sprites

struct struct_Vector2 dir_to_vec(int direction);
int vec_to_dir(struct struct_Vector2 v);
int ints_to_dir(int x, int y);

#endif //DIRECTION_H