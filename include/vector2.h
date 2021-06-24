#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct struct_Vector2 {
	int x;
	int y;
} Vector2;

inline void vec2_set(Vector2 *v, int x, int y) { v->x = x; v->y = y; }

#endif //VECTOR2_H