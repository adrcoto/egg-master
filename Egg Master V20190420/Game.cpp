#include "Game.h"
#include "TextureManager.h"
#include "Background.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <time.h>
#include "PlayerStats.h"


#define EGG true
#define ROCK false

bool Game::isRunning = false;

const int Game::BOX_WIDTH = 120;
const int Game::BOX_HEIGHT = 80;

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



int* Game::initStages(int number, int stage) {
	int* temp = new int[number];
	for (int i = 0; i < number; i++) 
		temp[i] = stage * (1 + i);

	return temp;
}

int* Game::initArray(int dim) {
	int* temp = new int[dim];
	for (int i = 0; i < dim; i++)
		temp[i] = 0;

	return temp;
}

void Game::init(int nrEggs = 65, int nrRocks = 15) {
	this->window = nullptr;
	this->renderer = nullptr;
	this->timeAnouncer = 0;

	this->healthStages = initStages(50, 10);
	this->checkedHealthStages = initArray(50);
	this->cleanseStages = initStages(20, 25);
	this->checkedCleanseStages = initArray(50);

	this->eggNumber = nrEggs;
	this->rockNumber = nrRocks;
}



/*
	Game constructors
*/
Game::Game(){
	init();
}

Game::Game(int nrEggs) {
	init(nrEggs);
}

Game::Game(int nrEggs, int nrRocks) {
	init(nrEggs, nrRocks);
}


/*
	Game deconstructor
*/
Game::~Game(){


}

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

		for (int i = 0; i < eggNumber; i++)
			assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "egg", EGG);
		
		for (int i = 0; i < 15; i++)
			assets->CreateProjectile(Vector2D(rand() % (WIDTH - 36), rand() % (maxHeight + 1 - minHeight) + minHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "rock", ROCK);


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

	SDL_Color black = { 0, 0, 0 };

	collectedEggs.addComponent<UILabel>(85, 25, "", "consolas", black);
	brokenEggs.addComponent<UILabel>(85, 75, "", "consolas", black);
	anouncer.addComponent<UILabel>(WIDTH / 2 - 80, HEIGHT / 2 - 200, "", "consolas", black);


	assets->addFX("nope", "assets/sound/nope.wav");
	assets->addFX("collect", "assets/sound/collect.wav");
	assets->addFX("drop", "assets/sound/drop.wav");
	assets->addFX("cleanse", "assets/sound/cleanse.wav");
	assets->addFX("heal", "assets/sound/heal.wav");
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


	//loops through all colliders and check for collisiobs (Axis aligned bounding box)
	for (auto cc : colliders)
		if (cc->tag != "crate")
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc))
				if (cc->tag == "rock") {

					Mix_PlayChannel(-1, assets->getFX("nope"), 0);
					cc->transform->position = Vector2D(width, height);

					if (!PlayerStats::gameOver) {
						PlayerStats::rockHit();

						switch (PlayerStats::Lives()) {

						case 3: lives.getComponent<SpriteComponent>().setTexture("lives2");			
							break;

						case 2: lives.getComponent<SpriteComponent>().setTexture("lives1");				
							break;

						case 1: lives.getComponent<SpriteComponent>().setTexture("lives");						
							break;

						default: lives.getComponent<SpriteComponent>().setTexture("lives");
							break;
						}
					}			
				}//ends if there is collision with 'rock' entity

				else if (cc->tag == "egg") {
					Mix_PlayChannel(-1, assets->getFX("collect"), 0);
					cc->transform->position = Vector2D(width, height);
					if (!PlayerStats::gameOver) {
						PlayerStats::eggCollected();
			
						if (PlayerStats::eggsCollected() % cleanseStages[cleanseStage] == 0 && checkedCleanseStages[cleanseStage] == 0) {
							Mix_PlayChannel(-1, assets->getFX("cleanse"), 0);
							PlayerStats::cleanse(2.0f);
							checkedCleanseStages[cleanseStage] == 1;
							cleanseStage += 1;
						}

						//% 75, 150, 225, 300, 375, 425
						if (PlayerStats::eggsCollected() % healthStages[healthStage] == 0 && checkedHealthStages[healthStage] == 0) {			
							if (PlayerStats::canHeal()) { //if can heal increase health by one				
								Mix_PlayChannel(-1, assets->getFX("heal"), 0);
								PlayerStats::healUp();
								if (PlayerStats::Lives() == 3) 
									lives.getComponent<SpriteComponent>().setTexture("lives2");											
								else 
									lives.getComponent<SpriteComponent>().setTexture("lives1");

								checkedHealthStages[healthStage] = 1;
								healthStage += 1;
							}
							else { // if lives == 3, then borkenEggs /= 1.2						
								PlayerStats::cleanse(1.2f);
								Mix_PlayChannel(-1, assets->getFX("cleanse"), 0);
								healthStage += 1;
							}
										
						}//ends if collected are divisible by 75, 150, 225, 300, 375, 425

					}//ends if is not game over

				}//ends if there is collision with 'egg' entity
	
		

	//game over
	gameOver();
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


void Game::gameOver() {
	if (PlayerStats::eggsCollected() >= 500) {
		//write congrats
		if (!PlayerStats::win)
			timeAnouncer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::win = true;
	}


	if (!PlayerStats::hasLives() || PlayerStats::eggsBroken() >= 10000) {
		//write game over
		if (!PlayerStats::gameOver)
			timeAnouncer = SDL_GetTicks() + 3000;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::gameOver = true;
	}
}

bool Game::running(){ return this->isRunning; }
