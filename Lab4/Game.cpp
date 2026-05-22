#pragma once
#include "Game.h"
#include "PlayerController.h"
#include "EnemyManager.h"
#include "TextManager.h";
#include "MediaManager.h";
#include "Text.h";
#include <string>

Uint32 lastTicks;


int frame;

const int Game::WIDTH = 1024;
const int Game::HEIGHT = 720;

const int Game::playerZoneY = (Game::HEIGHT * .75f) - 100;

Uint32 Game::score = 10000;
Text* Game::liveScoreText = NULL;

bool Game::displayGizmos = false;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
float Game::deltaTime;
bool Game::gamePaused = false;

PlayerController* Game::player;
EnemyManager* Game::enemyManager;


Game::Game() {

}

Game::~Game() {
	TextManager::CleanUp();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		lastTicks = SDL_GetTicks();
		std::cout << "SDL initialised!" << std::endl;
		int flags = 0;
		if (fullscreen) {
			flags = SDL_WINDOW_FULLSCREEN;
		}
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 20, 20,20 , 255);
			std::cout << "Renderer created!" << std::endl;
		}

		
		MediaManager::Init();

		
		TextManager::Init();

		isRunning = true;

		
		MediaManager::LoadAudio("assets/explosion.wav", "explosion");
		MediaManager::LoadAudio("assets/laser.wav", "laser_shooting");
		MediaManager::LoadAudio("assets/laser_enemy.wav", "enemy_shooting");
		MediaManager::LoadAudio("assets/win.wav", "win");
		MediaManager::LoadAudio("assets/gameover.wav", "gameover");
		MediaManager::LoadAudio("assets/damage.wav", "damage");





		player = new PlayerController("assets/player_anim.png", Game::WIDTH/2, Game::HEIGHT - 200);
		
		enemyManager = new EnemyManager(55);
		enemyManager->Init();

		
		liveScoreText = TextManager::AddText(Game::WIDTH * .85, Game::HEIGHT - 40, std::string("Score: ").append(std::to_string(score)).c_str());
	}
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {
	Input::SetKeystate();

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}

}
bool done = false;
void Game::update() {
	
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), lastTicks + 2))
		;
	deltaTime = (SDL_GetTicks() - lastTicks)/1000.0f;
	lastTicks = SDL_GetTicks();
	
	
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}


	
	GameObject::UpdateEverything();

	frame++;
	

}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);

	GameObject::RenderEverything();
	
	TextManager::Render();

	
	if (displayGizmos) {
		SDL_RenderDrawLine(renderer, 0, Game::playerZoneY, Game::WIDTH, Game::playerZoneY);
	}

	SDL_RenderPresent(renderer);

	
}

void Game::clean() {
	MediaManager::CleanUp();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}

void Game::GameOver()
{
	for (GameObject * go : GameObject::gameobjects) {
		GameObject::DestroyGameObject(go, 10);

	}

	delete liveScoreText;
	liveScoreText = NULL;
	
	MediaManager::PlayAudioOnce("gameover");
	Text* gameOverText = TextManager::AddText(WIDTH / 2, HEIGHT / 2, "Game Over!");
	gameOverText->SetScale(2 ,2);
}


void Game::GameWin()
{
	for (GameObject* go : GameObject::gameobjects) {
		GameObject::DestroyGameObject(go, 10);
	}
	delete liveScoreText;
	liveScoreText = NULL;
	
	MediaManager::PlayAudioOnce("win");
	score = score * player->GetHealth(); 
	Text* gameWinText = TextManager::AddText(WIDTH / 2, HEIGHT / 2, "You won the game!");
	gameWinText->SetScale(2, 2);
	Text* scoreText = TextManager::AddText(WIDTH / 2, HEIGHT / 2 + 50, std::string("Score: ").append(std::to_string(score)).c_str());
	scoreText->SetScale(1.2f, 1.2f);
}

void Game::ReduceScore(int reduction)
{
	if (score > 0)
	{
		if (reduction >= score) {
			score = 0;
		}
		else {
			score -= reduction;
		}
		
		liveScoreText->ChangeText(std::string("Score: ").append(std::to_string(score)).c_str());

		
	}

}

void Game::GrowScore(int grow)
{	
	score += grow;		
	liveScoreText->ChangeText(std::string("Score: ").append(std::to_string(score)).c_str());

}