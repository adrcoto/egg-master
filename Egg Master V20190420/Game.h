#pragma once
#include "SDL.h";
#include "SDL_image.h";

#include <iostream>

using namespace std;

class Game {
private:
	SDL_Window* window;

	bool isRunning;

public:
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static const int WIDTH, HEIGHT;

	Game();
	~Game();

	void init(const char*, int, int, int, int, bool);
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running();
};

