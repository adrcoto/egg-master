#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect collider = { 0, 0, 0, 0};
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}
	
	void init() override
	{
		/*if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}*/

		transform = &entity->getComponent<TransformComponent>();
		Game::colliders.push_back(this);
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height;
	}


};