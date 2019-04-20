#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"

GameObject* player;

SDL_Renderer* Game::renderer = nullptr;

/*
	Game constructor
*/
Game::Game(){
	this->window = nullptr;
	this->renderer = nullptr;
	this->isRunning = false;
}


/*
	Game deconstructor
*/
Game::~Game(){}

/*
	Initialize SDL
*/
void Game::init(const char* title, int posX, int posY, int with, int height, bool fullscreen) {

	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	//initalize all SDL subsystenms
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "Subsistems initialized ..." << endl;

		//create window
		this->window = SDL_CreateWindow(title, posX, posY, with, height, flags);

		//check if windows is created
		if (this->window)
			cout << "Window created ..." << endl;

		//create renderer
		renderer = SDL_CreateRenderer(this->window, -1, 0);

		//check if renderer is created
		if (this->renderer) {
			cout << "Renderer created ..." << endl;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		//start the game loop
		this->isRunning = true;
	} // ends if sdl is initialized

	player = new GameObject("assets/player.png", 0, 0);
}

/*
	Handle game events
*/
void Game::handleEvents(){

	//create the event object
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {

		//when press x
	case SDL_QUIT:
	
		//end game loop
		this->isRunning = false;
		break;

	default:
		break;
	}
}   


/*
	Update game states
*/
void Game::update() {
	player->update();
}

void Game::render() {

	SDL_RenderClear(this->renderer);

	//this is where we add stuff to render
	player->render();

	SDL_RenderPresent(renderer);
}


/*
	Destroy SDL objects
*/
void Game::clean() {
	//window
	SDL_DestroyWindow(this->window);

	//renderer
	SDL_DestroyRenderer(this->renderer);
	
	//quit SDL
	SDL_Quit();
	cout << "Game cleaned ..." << endl;
}

bool Game::running(){ return this->isRunning; }
