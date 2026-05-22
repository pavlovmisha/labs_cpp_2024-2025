#pragma once
#include "SDL.h"
#include <iostream>
#include "TextureManager.h"
#include "Input.h"

class PlayerController;
class EnemyManager;

class Text;
class Game
{
	public:
		Game();
		~Game();

		void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

		void handleEvents();
		void update();
		void render();
		void clean();

		static void GameOver();
		static void GameWin();


		bool running() { return isRunning; }

		const static int WIDTH, HEIGHT;
		static SDL_Renderer* renderer;
		static SDL_Event event;
		static float deltaTime;
		static const int playerZoneY;
		static Uint32 score;
		static void ReduceScore(int reduction);
		static void GrowScore(int grow);
		static Text* liveScoreText;

		static bool gamePaused;

		static bool displayGizmos;
		static PlayerController* player;
		static EnemyManager* enemyManager;

	private:
		bool isRunning;
		SDL_Window* window;
		
		
};

