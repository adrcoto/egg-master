#pragma once
#include "SDL.h";
#include "SDL_image.h";
#include <iostream>
#include <vector>
#include <sstream>


class AssetManager;
class ColliderComponent;

using namespace std;

class Game {
private:
	SDL_Window* window;
	

	int eggs = 75;

	//rand() % (WIDTH - 36)
	//rand() % (maxSpeed - minSpeed + 1) + minSpeed
	//rand() % (maxHeight + 1 - minHeight) + minHeight

	stringstream ss;
		

public:
	static bool isRunning;
	static const int BOX_WIDTH, BOX_HEIGHT;

	static const int minSpeed, maxSpeed;
	static const int minHeight, maxHeight;

	int timeWin;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static vector<ColliderComponent*> colliders;
	static AssetManager* assets;
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

