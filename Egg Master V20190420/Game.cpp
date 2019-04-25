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

const int Game::minSpeed = 50;
const int Game::maxSpeed = 150;

const int Game::minHeight = -4500;
const int Game::maxHeight = -800;



Background* bg;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
vector<ColliderComponent*> Game::colliders;
AssetManager* Game::assets = new AssetManager(&manager);

auto& eggsIcon(manager.addEntity());
auto& droppedIcon(manager.addEntity());

auto& player(manager.addEntity());

auto& collectedEggs(manager.addEntity());
auto& brokenEggs(manager.addEntity());
auto& lives(manager.addEntity());
auto& anouncer(manager.addEntity());


/*
	Game constructor
*/
Game::Game(){
	this->window = nullptr;
	this->renderer = nullptr;
	eggs = 50;
}

Game::Game(int nrEggs) {
	eggs = nrEggs;
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

		assets->addTexture("lives0", "assets/lives0.png");
		assets->addTexture("lives", "assets/lives.png");
		assets->addTexture("lives1", "assets/lives1.png");
		assets->addTexture("lives2", "assets/lives2.png");

		assets->addTexture("player", "assets/player.png");
		assets->addTexture("egg", "assets/egg.png");
		assets->addTexture("rock", "assets/rock.png");


		eggsIcon.addComponent<TransformComponent>(10, 10, 58, 47, 1);
		eggsIcon.addComponent<SpriteComponent>("eggIcon");

		droppedIcon.addComponent<TransformComponent>(10, 70, 58, 39, 1);
		droppedIcon.addComponent<SpriteComponent>("droppedIcon");

		lives.addComponent<TransformComponent>(10, 97, 108, 86, 1);
		lives.addComponent<SpriteComponent>("lives2");

		for (int i = 0; i < 65; i++)
			assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "egg");

		for (int i = 0; i < 15; i++)
			assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "rock");

		player.addComponent<TransformComponent>(WIDTH / 2.0 - 60.0f, 100.0f, 80, 80, 1.5f);
		player.addComponent<SpriteComponent>("player");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("crate");

	} // ends if sdl is initialized
	else
		cout << "SDL not loaded ..." << endl;

	if (TTF_Init() == -1)
		cout << "Something wrong with font";


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		cout << "Something wrong with sound";


	assets->addFont("consolas", "assets/font/consola.ttf", 35);
	assets->addFont("consola", "assets/font/consolas.ttf", 35);
	
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };

	collectedEggs.addComponent<UILabel>(85, 25, "", "consola", black);
	brokenEggs.addComponent<UILabel>(85, 75, "", "consola", black);
	anouncer.addComponent<UILabel>(WIDTH / 2 - 80, HEIGHT / 2 - 200, "", "consolas", black);
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
	collectedEggs.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());

	ss << PlayerStats::eggsBroken();
	brokenEggs.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
	ss.str(string());

	if (PlayerStats::gameOver) {
		ss << "You Lost!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());
	}

	if (PlayerStats::win) {
		ss << "You Won!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());
	}

	int width = rand() % (Game::WIDTH - 35);
	int height = rand() % (Game::maxHeight + 1 - (Game::minHeight - 500)) + Game::minHeight;

	for (auto cc : colliders)
		if (cc->tag != "crate")
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc))
				if (cc->tag == "rock") {		
					Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/nope.wav"), 0);
				
						PlayerStats::rockHit();
						if (PlayerStats::Lives() == 2)
							lives.getComponent<SpriteComponent>().setTexture("lives1");
						else if (PlayerStats::Lives() == 1)
							lives.getComponent<SpriteComponent>().setTexture("lives");
						else if (PlayerStats::Lives() == 3)
							lives.getComponent<SpriteComponent>().setTexture("lives2");
						else 
							lives.getComponent<SpriteComponent>().setTexture("lives0");

						cc->transform->position = Vector2D(width, height);
				}

				else if (cc->tag == "egg") {
					Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/collect.wav"), 0);
					cc->transform->position = Vector2D(width, height);
					if (!PlayerStats::gameOver)
					PlayerStats::eggCollected();					
				}


	if (PlayerStats::eggsCollected() >= 500) {
		//write congrats
		if (!PlayerStats::win)
			timeAnouncer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::win = true;
	}

	if (PlayerStats::eggsCollected() >= 10) {
		PlayerStats::healUp();

		if (PlayerStats::Lives() == 2)
			lives.getComponent<SpriteComponent>().setTexture("lives1");

		else if (PlayerStats::Lives() == 3)
			lives.getComponent<SpriteComponent>().setTexture("lives2");
	}

	cout << PlayerStats::Lives() << endl;

	if (!PlayerStats::hasLives() || PlayerStats::eggsBroken() >= 100) {
		//write game over
		if (!PlayerStats::gameOver)
			timeAnouncer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::gameOver = true;
	}
}



void Game::render() {

	SDL_RenderClear(this->renderer);
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
