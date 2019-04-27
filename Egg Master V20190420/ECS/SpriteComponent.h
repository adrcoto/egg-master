#pragma once
#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "../AssetManager.h"


class SpriteComponent : public Component{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect src, dest;

	string currentText;
	string prevText;
public:
	SpriteComponent() = default;

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}


	SpriteComponent(string id) {
		setTexture(id);
	}

	void setTexture(string id) {	
		if (textChanged(id)) 
			texture = Game::assets->getTexture(id);
	}

	bool textChanged(string text) {
		prevText = currentText;
		currentText = text;

		return (prevText != currentText);
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