#pragma once
#include "SDL.h";
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "Frame.h"
#include <thread>
#include "Timer.h"

using namespace std;

class AssetManager;
class ColliderComponent;

class Game {
private:
	thread soundThread, hudThread, addEggThread, addRockThread;
	SDL_Window* window;

	//number of eggs/rocks
	int eggNumber;
	int rockNumber;

	//time for showing messages
	int timeAnouncer;

	//stream for score
	stringstream ss;

	int randWidth = 0;
	int randHeight = 0;

	int cleanseStage = 0, healthStage = 0;
	int* cleanseStages, *healthStages;
	int* checkedCleanseStages, * checkedHealthStages;
	
	

	int* initStages(int, int);
	int* initArray(int);
	void init(int, int);

	void loadEggs();
	void loadRocks();

	void updateHud(int, int, int, string, stringstream&);
	void checkForCollision();
	void gameOver(int, int, int);

	void joinThread(thread&);
public:
	void initImages();
	void initHud();
	void initSound();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static vector<ColliderComponent*> colliders;
	static AssetManager* assets;


	static bool cap;

	//window dimension
	static const int WIDTH, HEIGHT;

	//player crate dimension
	static const int BOX_WIDTH, BOX_HEIGHT;

	static bool isRunning;
	
	//min-max speed for projectiles
	static const int minSpeed, maxSpeed;

	//min-max spawn point for projectiles
	static const int minHeight, maxHeight;

	Game();
	Game(int);
	Game(int, int);
	~Game();

	void init(const char*, int, int, int, int, bool);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running();
};

