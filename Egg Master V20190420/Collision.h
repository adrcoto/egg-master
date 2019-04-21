#pragma once
#include "SDL.h"

/**/
class Collision {
public:
	Collision();
	~Collision();

	//Axis Aligned Bounding Box - AABB
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
};

