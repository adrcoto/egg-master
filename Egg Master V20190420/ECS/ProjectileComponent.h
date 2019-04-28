#pragma once
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include "../PlayerStats.h"

class ProjectileComponent : public Component {
protected:
	TransformComponent* transform;
	Vector2D velocity;
	int range = 0, speed = 0;
	int width = 0, height = 0;
public:
	ProjectileComponent(int sp, Vector2D vel) :  speed(sp), velocity(vel) {
		if (speed <= 0)
			speed = 1;
	}
	~ProjectileComponent() {}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity * speed;
		
	}


	void update() override {
		if (transform->position.y >= Game::HEIGHT - 55) {
			speed = rand() % (Game::maxSpeed - Game::minSpeed + 1);
			width = rand() % (Game::WIDTH - 35);
			height = rand() % (Game::maxHeight + 1 - Game::minHeight) + Game::minHeight;

			entity->getComponent<ProjectileComponent>().speed = speed;
			entity->getComponent<ProjectileComponent>().transform->position = Vector2D(width, height);
		}
	}
};