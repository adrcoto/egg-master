#include "Game.h"
#include "TextureManager.h"
#include "Background.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

constexpr auto BOX_WIDTH = 120;
constexpr auto BOX_HEIGHT = 80;

const int Game::WIDTH = 1366;
const int Game::HEIGHT = 768;


Background* bg;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.addEntity());
auto& egg(manager.addEntity());

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

		bg = new Background();

		bg->load("assets/background.png");

		player.addComponent<TransformComponent>(WIDTH / 2 - 60, 100, 80, 80, 1.5);
		player.addComponent<SpriteComponent>("assets/player.png");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("crate");

		egg.addComponent<TransformComponent>(WIDTH / 2, 100);
		egg.addComponent<SpriteComponent>("assets/egg.png");
		egg.addComponent<ColliderComponent>("egg");

	} // ends if sdl is initialized
	else
		cout << "SDL not loaded ..." << endl;
}

/*
	Handle game events
*/
void Game::handleEvents() {

	//create the event object
	
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT)
		isRunning = false;

	else {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			default:
				break;
			}
		}
	}
}

/*
	Update game states
*/
void Game::update() {
	manager.refresh();
	manager.update();

	if (player.getComponent<TransformComponent>().position.x > Game::WIDTH - BOX_WIDTH)
		player.getComponent<TransformComponent>().position.x = Game::WIDTH - BOX_WIDTH;

	else if (player.getComponent<TransformComponent>().position.x < 0)
		player.getComponent<TransformComponent>().position.x = 0;

	if (player.getComponent<TransformComponent>().position.y > Game::HEIGHT - BOX_HEIGHT - 30)
		player.getComponent<TransformComponent>().position.y = Game::HEIGHT - BOX_HEIGHT - 30;

	else if (player.getComponent<TransformComponent>().position.y < 0)
		player.getComponent<TransformComponent>().position.y = 0;

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		egg.getComponent<ColliderComponent>().collider)) {
		egg.getComponent<SpriteComponent>().setTexture("assets/rock.png");
		cout << "The egg has been collected ..." << endl;
	}
	else {
		egg.getComponent<SpriteComponent>().setTexture("assets/egg.png");
	}
}

void Game::render() {

	SDL_RenderClear(this->renderer);

	//this is where we add stuff to render
	bg->draw();
	manager.draw();
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
