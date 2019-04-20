#include "GameObject.h"
#include "TextureManager.h"



GameObject::GameObject() {
	this->xPos = 0;
	this->yPos = 0;

	this->srcRect.h = 0;
	this->srcRect.w = 0;
	this->srcRect.x = 0;
	this->srcRect.y = 0;

	this->destRect.h = 0;
	this->destRect.w = 0;
	this->destRect.x = 0;
	this->destRect.y = 0;

	this->objYexture = nullptr;
}

GameObject::GameObject(const char* texture, int x, int y) {
	this->srcRect.h = 0;
	this->srcRect.w = 0;
	this->srcRect.x = 0;
	this->srcRect.y = 0;

	this->destRect.h = 0;
	this->destRect.w = 0;
	this->destRect.x = 0;
	this->destRect.y = 0;

	this->objYexture = TextureManager::loadTexture(texture);

	this->xPos = x;
	this->yPos = y;
}

GameObject::~GameObject() {}

void GameObject::update() {
	xPos += 1;
	yPos += 1;

	srcRect.h = 80;
	srcRect.w = 80;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, objYexture, &srcRect, &destRect);
}
