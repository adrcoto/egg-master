#pragma once
#include "Game.h"
class GameObject {
private:
	int xPos, yPos;

	SDL_Texture* objYexture;
	SDL_Rect srcRect, destRect;


public:
	GameObject();
	GameObject(const char*, int, int);
	~GameObject();

	void update();
	void render();
};

