#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component {
public:

	Vector2D position;
	Vector2D velocity;

	int height = 36;
	int width = 36;
	float scale = 1.0;

	int speed = 5;
	
	TransformComponent() {
		position.x = position.y = 0.0f;
	}

	TransformComponent(float scale) {
		position.x = 0.0f;
		position.y = 0.0f;
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
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};