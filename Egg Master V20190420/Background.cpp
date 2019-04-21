#include "Background.h"
#include "TextureManager.h"


Background::Background(){
	
	this->texture = nullptr;

	this->src.x = this->src.y = 0;
	this->dest.x = this->dest.y = 0;

	this->src.h = this->dest.h = Game::HEIGHT;
	this->src.w = this->dest.w = Game::WIDTH;
}


Background::~Background(){
	SDL_DestroyTexture(texture);
}


void Background::load(const char* path) {
	this->texture = TextureManager::loadTexture(path);
}

void Background::draw() {
	TextureManager::draw(this->texture, this->src, this->dest);
}