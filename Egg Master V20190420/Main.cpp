#include "Game.h";
#include "TextureManager.h"
#include "Timer.h"


Game* game = nullptr;

#define EGGS 75
#define ROCKS 17
const int FPS = 144;

Timer fps, update, lock;

int main(int args, char* argcv[]) {
	game = new Game(EGGS, ROCKS);


	game->init("Egg Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::WIDTH, Game::HEIGHT, false);
	
	Frame* f = new Frame(FPS);

	update.start();
	fps.start();
	while (game->running()) {
		lock.start();
		
		game->handleEvents();
		game->update();
		game->render();

		f->incrementFrame();

		if (Game::cap)
			f->setCap(lock.get_ticks());

		if (f->setInterval(200, update.get_ticks(), fps.get_ticks()))
			update.start();

		Timer::setDelta(lock);

	}
	game->clean();

	return 0;
}
