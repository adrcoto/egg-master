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

	// Limits
	int x1 = -1;
	int x2 = -1;
	int y1 = -1;
	int y2 = -1;

	TransformComponent() {
		position.zero();
		x1 = -1;
		x2 = -1;
		y1 = -1;
		y2 = -1;
	}

	TransformComponent(float scale) {
		position.zero();
		this->scale = scale;
		x1 = -1;
		x2 = -1;
		y1 = -1;
		y2 = -1;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
		x1 = -1;
		x2 = -1;
		y1 = -1;
		y2 = -1;
	}

	TransformComponent(float x, float y, int w, int h, float scale) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		this->scale = scale;
		x1 = -1;
		x2 = -1;
		y1 = -1;
		y2 = -1;
	}

	
	void init() override {
		velocity.zero();
	}
	
	void update() override {
		position.x += velocity.x * speed * Timer::deltaTime();
		position.y += velocity.y * speed * Timer::deltaTime();

		if (x1 != -1 || x2 != -1)
			position.x = clamp(position.x, x1, x2);
		if (y1 != -1 || y2 != -1)
			position.y = clamp(position.y, y1, y2);
	}

	void setLimits(int x1, int x2, int y1, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	int clamp(int value, int min, int max) {
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}
};