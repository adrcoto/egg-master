#pragma once
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include "../PlayerStats.h"

class ProjectileComponent : public Component {
private:
	TransformComponent* transform;
	Vector2D velocity;

	int range = 0, speed = 0, distance = 0;

	int width = 0, height = 0;

public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel) {
		if (speed <= 0)
			speed = 1;
	}
	~ProjectileComponent() {

	}

	void init() override {
		
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity * speed;
		//cout << transform->position;
		if (transform->position.x == 0)
			cout << "aaaa" << endl;

		if (transform->position.x >= Game::WIDTH - 36)
			cout << "\t\tbbbb" << endl;

		cout << transform->velocity << endl;
		cout << transform->position << endl;
	}


	void update() override {
		distance += speed;
		
		//if (distance > range) 
		//	entity->destroy();
		//else if (
		//	transform->position.x > Game::WIDTH ||
		//	transform->position.x < 0 ||
		//	transform->position.y > Game::HEIGHT ||
		//	transform->position.y < 0
		//	) entity->destroy();

		speed = rand() % (Game::maxSpeed - Game::minSpeed + 1);
		width = rand() % (Game::WIDTH - 35);
		height = rand() % (Game::maxHeight + 1 - Game::minHeight) + Game::minHeight;

		if (transform->position.y >= Game::HEIGHT - 55) {
			PlayerStats::eggDropped();
			entity->getComponent<ProjectileComponent>().speed = speed;
			entity->getComponent<ProjectileComponent>().transform->position = Vector2D(width, height);
		}
	}
};