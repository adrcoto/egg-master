#include "Game.h"
#include "TextureManager.h"
#include "Background.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <time.h>
#include "PlayerStats.h"
#include "GameStats.h"


#define EGG true
#define ROCK false

bool Game::isRunning = false;
bool Game::cap = true;


const int Game::BOX_WIDTH = 120;
const int Game::BOX_HEIGHT = 80;

const int Game::WIDTH = 1366;
const int Game::HEIGHT = 768;

const int Game::minSpeed = 50;
const int Game::maxSpeed = 200;

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
auto& fps(manager.addEntity());


void Game::init(int nrEggs = 65, int nrRocks = 15) {
	this->window = nullptr;
	this->renderer = nullptr;
	this->timeAnouncer = 0;

	this->healthStages = initStages(6, 75);
	this->checkedHealthStages = initArray(6);
	this->cleanseStages = initStages(4, 100);
	this->checkedCleanseStages = initArray(4);

	this->eggNumber = nrEggs;
	this->rockNumber = nrRocks;
}

/*
	Game constructors
*/
Game::Game() { init(); }

Game::Game(int nrEggs) { init(nrEggs); }

Game::Game(int nrEggs, int nrRocks) { init(nrEggs, nrRocks); }

/*
	Game deconstructor
*/
Game::~Game() {
	delete[] healthStages;
	delete[] checkedHealthStages;
	delete[] cleanseStages;
	delete[] checkedCleanseStages;
}


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

void Game::initSound() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		cout << "Something wrong with sound";

	assets->addFX("nope", "assets/sound/nope.wav");
	assets->addFX("collect", "assets/sound/collect.wav");
	assets->addFX("drop", "assets/sound/drop.wav");
	assets->addFX("cleanse", "assets/sound/cleanse.wav");
	assets->addFX("heal", "assets/sound/heal.wav");
}

void Game::initHud() {
	assets->addTexture("egg", "assets/egg.png");
	assets->addTexture("rock", "assets/rock.png");
	assets->addTexture("player", "assets/player.png");

	assets->addTexture("eggIcon", "assets/collectedEggs.png");
	assets->addTexture("droppedIcon", "assets/brokenEggs.png");

	assets->addTexture("lives0", "assets/lives0.png");
	assets->addTexture("lives", "assets/lives.png");
	assets->addTexture("lives1", "assets/lives1.png");
	assets->addTexture("lives2", "assets/lives2.png");

	if (TTF_Init() == -1)
		cout << "Something wrong with font";
	else {
		assets->addFont("consolas", "assets/font/consola.ttf", 35);

		SDL_Color black = { 0, 0, 0 };
		SDL_Color yellow = {255, 255, 0};
		collectedEggs.addComponent<UILabel>(85, 25, "", "consolas", black);

		brokenEggs.addComponent<UILabel>(85, 75, "", "consolas", black);

		anouncer.addComponent<UILabel>(WIDTH / 2 - 80, HEIGHT / 2 - 200, "", "consolas", black);

		eggsIcon.addComponent<TransformComponent>(10, 10, 58, 47, 1);
		eggsIcon.addComponent<SpriteComponent>("eggIcon");

		droppedIcon.addComponent<TransformComponent>(10, 70, 58, 39, 1);
		droppedIcon.addComponent<SpriteComponent>("droppedIcon");

		lives.addComponent<TransformComponent>(10, 97, 108, 86, 1);
		lives.addComponent<SpriteComponent>("lives2");

		fps.addComponent<UILabel>(WIDTH - 50, 10, "", "consolas", yellow);
		fps.addComponent<TransformComponent>();
	}
}


void Game::loadEggs() {
	for (int i = 0; i < eggNumber; i++) {
		if (PlayerStats::gameOver)
			return;
	
		randWidth = rand() % (WIDTH - 36);
		randHeight = rand() % (maxHeight + 1 - minHeight) + minHeight;
		assets->CreateProjectile(Vector2D(randWidth, randHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "egg", EGG);
		this_thread::sleep_for(chrono::milliseconds(Timer::timeToDelay + 10));
		cout << "Created egg nr " << i + 1 << " -> (" << randWidth << ", " << 100 << "), delay -> " << Timer::timeToDelay + 10 << endl;
	}
}

void Game::loadRocks() {
	for (int i = 0; i < rockNumber; i++) {
		if (PlayerStats::gameOver)
			return;
		
		assets->CreateProjectile(Vector2D(randWidth, randHeight), Vector2D(0, 1), rand() % (maxSpeed - minSpeed + 1) + minSpeed, "rock", ROCK);
		this_thread::sleep_for(chrono::milliseconds(Timer::timeToDelay + 10));
		cout << "Created rock nr " << i + 1 <<" -> (" << randWidth << ", " << 100 << "), delay -> " << Timer::timeToDelay + 10 << endl;
	}
}




void Game::updateHud(int collected_eggs, int broken_eggs, int FPS, string fontname, stringstream& ss) {
	ss << collected_eggs;
	collectedEggs.getComponent<UILabel>().setLabelText(ss.str(), fontname);
	ss.str(string());

	ss << broken_eggs;
	brokenEggs.getComponent<UILabel>().setLabelText(ss.str(), fontname);
	ss.str(string());

	ss << FPS;
	fps.getComponent<UILabel>().setLabelText(ss.str(), fontname);
	ss.str(string());
}

void Game::checkForCollision() {
	for (auto cc : colliders) {
		if (cc->tag != "crate") {
			if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)) {

				randWidth = rand() % (Game::WIDTH - 35);
				randHeight = rand() % (Game::maxHeight + 1 - (Game::minHeight - 500)) + Game::minHeight;

				if (cc->tag == "rock") {
					Mix_PlayChannel(-1, assets->getFX("nope"), 0);
					cc->transform->position = Vector2D(randWidth, randHeight);

					if (!PlayerStats::gameOver) {
						PlayerStats::rockHit();

						switch (PlayerStats::Lives()) {

						case 3: lives.getComponent<SpriteComponent>().setTexture("lives2");
							break;

						case 2: lives.getComponent<SpriteComponent>().setTexture("lives1");
							break;

						case 1: lives.getComponent<SpriteComponent>().setTexture("lives");
							break;

						default: lives.getComponent<SpriteComponent>().setTexture("lives0");
							break;
						}
					}
				}//ends if there is collision with 'rock' entity

				else if (cc->tag == "egg") {
					Mix_PlayChannel(-1, assets->getFX("collect"), 0);
					cc->transform->position = Vector2D(randWidth, randHeight);
					if (!PlayerStats::gameOver) {
						PlayerStats::eggCollected();

						if (PlayerStats::eggsCollected() % cleanseStages[cleanseStage] == 0 && checkedCleanseStages[cleanseStage] == 0) {
							Mix_PlayChannel(-1, assets->getFX("cleanse"), 0);
							PlayerStats::cleanse(1.5f);
							checkedCleanseStages[cleanseStage] == 1;
							cleanseStage += 1;
						}

						//% 75, 150, 225, 300, 375, 425
						if (PlayerStats::eggsCollected() % healthStages[healthStage] == 0 && checkedHealthStages[healthStage] == 0) {
							if (PlayerStats::canHeal()) { //if can heal increase health by one				
								Mix_PlayChannel(-1, assets->getFX("heal"), 0);
								PlayerStats::heal();
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

			}//ends check if player collide with other entities

		}//ends exclude player from colliders

	}//ends looping through colliders components
}

/*Rules for winning / losing the game and show message*/
void Game::gameOver(int collectedEggs, int brokenEggs, int timeForMessage) {
	if (PlayerStats::eggsCollected() >= collectedEggs) {
		ss << "You Won!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());

		if (!PlayerStats::win)
			timeAnouncer = SDL_GetTicks() + timeForMessage;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::win = true;
	}
	else if (!PlayerStats::hasLives() || PlayerStats::eggsBroken() >= brokenEggs) {
		ss << "You Lost!";
		anouncer.getComponent<UILabel>().setLabelText(ss.str(), "consolas");
		ss.str(string());

		if (!PlayerStats::gameOver)
			timeAnouncer = SDL_GetTicks() + timeForMessage;
		if (SDL_GetTicks() > timeAnouncer)
			isRunning = false;

		PlayerStats::gameOver = true;
	}
}

void Game::joinThread(thread& th) {
	std::thread::id this_id = th.get_id();
	if (th.joinable()) {
		th.join();
		cout << this_id << " joined" << endl;
	}
	else
		cout << this_id << " has not finished yet!" << endl;
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
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		}
		
		soundThread = thread(&Game::initSound, this);
		hudThread = thread(&Game::initHud, this);

		//start the game loop
		Game::isRunning = true;

		bg = new Background();
		bg->load("assets/background.png");
		
		//player entity
		player.addComponent<TransformComponent>(WIDTH / 2.0 - 60.0f, HEIGHT - 60.0f, BOX_HEIGHT, BOX_HEIGHT, 1.5f);
		player.addComponent<SpriteComponent>("player");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("crate");

		addEggThread = thread(&Game::loadEggs, this);
		addRockThread = thread(&Game::loadRocks, this);

		//spawn eggs
		//loadEggs();

		//spawn rocks
		//loadRocks();

	} // ends if sdl is initialized
	else
		cout << "SDL not loaded ..." << endl;

	joinThread(soundThread);
	joinThread(hudThread);

} //ends Game::init();

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
			case SDLK_RETURN: cap = (!cap);
				break;
			case SDLK_ESCAPE : isRunning = false;
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
	
	m.lock();
	manager.refresh();
	manager.update();
	m.unlock();

	//loops through all colliders and check for collisiobs (Axis aligned bounding box)
	checkForCollision();

	updateHud(PlayerStats::eggsCollected(), PlayerStats::eggsBroken(), Frame::getFPS(), "consolas", ss);

	/*rules for winning / losing the game */
	gameOver(500, 75, 3000);

} //ends Game::update();

/*
	Display output
*/
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
	//statsThread.join();
	joinThread(soundThread);
	joinThread(hudThread);

	joinThread(addEggThread);
	joinThread(addRockThread);

	//imagesThread.join();
	cout << "Game cleaned ..." << endl;
}



bool Game::running(){ return this->isRunning; }
