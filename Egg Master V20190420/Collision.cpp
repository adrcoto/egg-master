#include "Collision.h"



Collision::Collision(){ }


Collision::~Collision(){}

//Axis Aligned Bounding Box - AABB
bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	return (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		);
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	return(AABB(colA.collider, colB.collider));
}
