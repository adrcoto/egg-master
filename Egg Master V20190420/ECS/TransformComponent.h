#pragma once
#include "Components.h"
#include "../Vector2D.h"
#include "../Game.h"
#include "../Timer.h"


class TransformComponent : public Component {
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
		position.x += velocity.x * speed * Timer::deltaTime();
		position.y += velocity.y * speed * Timer::deltaTime();
	}

};