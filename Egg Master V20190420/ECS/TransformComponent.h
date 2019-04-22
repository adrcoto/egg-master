#pragma once
#include "Components.h"
#include "../Vector2D.h"
#include "../Game.h"

class TransformComponent : public Component {
	int prevTime = 0;
	int curentTime = 0;
	float deltaTime = 0;
public:

	Vector2D position;
	Vector2D velocity;

	int height = 36;
	int width = 36;
	float scale = 1.0;

	int speed = 1;
	
	TransformComponent() {
		position.zero();
	}

	TransformComponent(float scale) {
		position.zero();
		this->scale = scale;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int w, int h, float scale) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		this->scale = scale;
	}

	
	void init() override {
		velocity.zero();
	}

	void update() override {
		
		prevTime = curentTime;
		curentTime = SDL_GetTicks();
		deltaTime = (curentTime - prevTime) / 1000.0f;

		position.x += velocity.x * speed * deltaTime;
		position.y += velocity.y * speed * deltaTime; 
	}

};