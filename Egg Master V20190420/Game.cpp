#include "Game.h"
#include "TextureManager.h"
#include "Background.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <time.h>

#include "PlayerStats.h"


const int Game::BOX_WIDTH = 120;
const int Game::BOX_HEIGHT = 80;

bool Game::isRunning = false;



const int Game::WIDTH = 1366;
const int Game::HEIGHT = 768;

const int Game::minSpeed = 80;
const int Game::maxSpeed = 170;

const int Game::minHeight = -2500;
const int Game::maxHeight = -700;



Background* bg;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
vector<ColliderComponent*> Game::colliders;
AssetManager* Game::assets = new AssetManager(&manager);

auto& eggsIcon(manager.addEntity());
auto& droppedIcon(manager.addEntity());

auto& player(manager.addEntity());
auto& egg(manager.addEntity());
auto& rock(manager.addEntity());

auto& aaa(manager.addEntity());
auto& bbb(manager.addEntity());

auto& lives(manager.addEntity());
auto& anouncer(manager.addEntity());
auto& tim(manager.addEntity());

/*
	Game constructor
*/
Game::Game(){
	this->window = nullptr;
	this->renderer = nullptr;
}


/*
	Game deconstructor
*/
Game::~Game(){}

/*
	Initialize SDL
*/
void Game::init(const char* title, int posX, int posY, int with, int height, bool fullscreen) {
	srand(time(NULL));
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
		Game::isRunning = true;

		bg = new Background();

		bg->load("assets/background.png");

		assets->addTexture("eggIcon", "assets/collectedEggs.png");
		assets->addTexture("droppedIcon", "assets/brokenEggs.png");

		assets->addTexture("player", "assets/player.png");
		assets->addTexture("egg", "assets/egg.png");
		assets->addTexture("rock", "assets/rock.png");


		eggsIcon.addComponent<TransformComponent>(10, 10, 58, 47, 1);
		eggsIcon.addComponent<SpriteComponent>("eggIcon");

		droppedIcon.addComponent<TransformComponent>(10, 70, 58, 39, 1);
		droppedIcon.addComponent<SpriteComponent>("droppedIcon");

		player.addComponent<TransformComponent>(WIDTH / 2.0 - 60.0f, 100.0f, 80, 80, 1.5f);
		player.addComponent<SpriteComponent>("player");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("crate");


		egg.addComponent<TransformComponent>(WIDTH / 2, 450);
		//egg.addComponent<SpriteComponent>("assets/egg.png");
		egg.addComponent<SpriteComponent>("egg");
		egg.addComponent<ColliderComponent>("egg");

		rock.addComponent<TransformComponent>(WIDTH / 2 + 100, 450);
		//	rock.addComponent<SpriteComponent>("assets/rock.png");
		rock.addComponent<SpriteComponent>("rock");
		rock.addComponent<ColliderComponent>("rock");


		/*assets->CreateProjectile(Vector2D(WIDTH - 36, 100), Vector2D(-1, 0), 0, 1, "egg");
		assets->CreateProjectile(Vector2D(WIDTH - 36, 200), Vector2D(-1, 0), 0, 2, "egg");
		assets->CreateProjectile(Vector2D(WIDTH - 36, 300), Vector2D(-1, 0), 0, 3, "egg");
		assets->CreateProjectile(Vector2D(WIDTH - 36, 400), Vector2D(-1, 0), 0, 2, "egg");*/

		
		
		
	
	} // ends if sdl is initialized
	else
		cout << "SDL not loaded ..." << endl;

	if (TTF_Init() == -1)
		cout << "Something wrong with font";

	assets->addFont("consolas", "assets/consola.ttf", 35);
	assets->addFont("consola", "assets/consolas.ttf", 25);
	
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };


	aaa.addComponent<UILabel>(85, 25, "", "consola", black);
	bbb.addComponent<UILabel>(85, 75, "", "consola", black);
	lives.addComponent<UILabel>(85, 135, "", "consola", black);
	tim.addComponent<UILabel>(85, 195, "", "consola", black);

	anouncer.addComponent<UILabel>(WIDTH / 2 - 80, HEIGHT / 2 - 200, "", "consolas", black);

	for (int i = 0; i < 25; i++)
		assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), 0, rand() % (maxSpeed - minSpeed + 1) + minSpeed, "egg");

	for (int i = 0;  i < 15; i++)
		assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), 0, rand() % (maxSpeed - minSpeed + 1) + minSpeed, "rock");
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


	ss << PlayerStats::eggsCollected();
	aaa.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());

	ss << PlayerStats::eggsBroken();
	bbb.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());


	ss << PlayerStats::Lives();
	lives.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());

	ss << SDL_GetTicks();
	tim.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());

	if (PlayerStats::gameOver) {
		ss << "You Lose!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());
	}

	if (PlayerStats::win) {
		ss << "You Won!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());
	}




	int width = rand() % (Game::WIDTH - 35);
	int height = rand() % (Game::maxHeight + 1 - Game::minHeight) + Game::minHeight;

	for (auto cc : colliders)
		if (cc->tag != "crate")
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc))
				if (cc->tag == "rock") {
					PlayerStats::rockHit();
					cc->transform->position = Vector2D(width, height);
				}

				else if (cc->tag == "egg") {
					PlayerStats::eggCollected();
					cc->transform->position = Vector2D(width, height);
				}



	if (PlayerStats::eggsCollected() >= 10) {
		//write congrats
		if (!PlayerStats::win)
			timeWin = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeWin)
			isRunning = false;

		PlayerStats::win = true;
	}


	if (!PlayerStats::hasLives() || PlayerStats::eggsBroken() >= 1000) {
		//write game over
		if (!PlayerStats::gameOver)
			timeWin = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeWin)
			isRunning = false;

		PlayerStats::gameOver = true;
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
