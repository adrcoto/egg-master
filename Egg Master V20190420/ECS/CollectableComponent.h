#pragma once
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include "../PlayerStats.h"
#include "../AssetManager.h"


class CollectableComponment : public ProjectileComponent {
private:
	int range = 0, speed = 0;
	int width = 0, height = 0;
public:
	CollectableComponment(int sp, Vector2D vel) : ProjectileComponent(sp, vel) {}
	~CollectableComponment() { }
	


	void update() override {

		speed = rand() % (Game::maxSpeed - Game::minSpeed + 1);
		width = rand() % (Game::WIDTH - 35);
		height = rand() % (Game::maxHeight + 1 - Game::minHeight) + Game::minHeight;

		if (transform->position.y >= Game::HEIGHT - 55) {
			PlayerStats::eggDropped();
			Mix_PlayChannel(-1, Game::assets->getFX("drop"), 0);
			entity->getComponent<CollectableComponment>().speed = speed;
			entity->getComponent<CollectableComponment>().transform->position = Vector2D(width, height);
		}
	}


};