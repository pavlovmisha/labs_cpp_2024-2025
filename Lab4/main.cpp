
#include "Game.h"
const int WIDTH = 640, HEIGHT = 480;

Game *game = nullptr;

int main(int argc, char* argv[]) {
	
	game = new Game();
	game->init("MyEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::WIDTH, Game::HEIGHT, false);

	while(game->running()) {
		
		game->handleEvents();
		
		game->update();
		game->render();

	};

	game->clean();
	return 0;
}