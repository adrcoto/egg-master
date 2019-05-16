#pragma once
#include "../Game.h"
#include "ECS.h"
#include "Components.h"

#define SPEED 800

class KeyboardController : public Component {
private:

public:
	TransformComponent* transform;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		transform->setLimits(0, Game::WIDTH - Game::BOX_WIDTH, int(Game::HEIGHT - 0.4f * Game::HEIGHT), Game::HEIGHT - Game::BOX_HEIGHT - 30);
	}

	void update() override {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				transform->velocity.y = -SPEED;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				transform->velocity.y = SPEED;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				transform->velocity.x = -SPEED;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				transform->velocity.x = SPEED;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				transform->velocity.y = 0;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				transform->velocity.y = 0;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				transform->velocity.x = 0;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			default:
				break;
			}
		}
	}

	int clamp(int value, int min, int max) {
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}
};
