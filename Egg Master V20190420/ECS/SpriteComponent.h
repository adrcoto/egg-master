#pragma once
#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"


class SpriteComponent : public Component{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect src, dest;


public:
	SpriteComponent() = default;

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	SpriteComponent(const char* path) {
		setTexture(path);
	}


	void setTexture(const char* path) {
		texture = TextureManager::loadTexture(path);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();

		src.x = src.y = 0;
		src.w = transform->width;
		src.h = transform->height;
	}

	void update() override {
		dest.x = static_cast<int> (transform->position.x);
		dest.y = static_cast<int> (transform->position.y);
		dest.w = transform->width * transform->scale;
		dest.h = src.h;
	}

	void draw() override {
		TextureManager::draw(texture, src, dest);
	}
};