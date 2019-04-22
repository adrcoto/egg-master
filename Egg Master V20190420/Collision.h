#pragma once
#include "SDL.h"
#include "ECS/ColliderComponent.h"

class ColliderComponent;

/**/
class Collision {
public:
	Collision();
	~Collision();

	//Axis Aligned Bounding Box - AABB
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent&, const ColliderComponent&);
};

