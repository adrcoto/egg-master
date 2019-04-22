#include "Game.h";
#include "TextureManager.h"

Game* game = nullptr;

//timpi per frame


int main(int args, char* argcv[]) {

	const int FPS = 144;
	const int frameDelay = 1000 / 144;

	Uint32 frameStart;
	int frameTime;

	game = new Game();

	game->init("Egg Master", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::WIDTH, Game::HEIGHT, false);

	while (game->running()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	return 0;
}