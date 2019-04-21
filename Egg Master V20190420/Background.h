#pragma once
#include "Game.h"

class Background {
private:
	SDL_Texture* texture;
	SDL_Rect src, dest;
public:
	Background();
	~Background();

	void load(const char*);
	void draw();
};

