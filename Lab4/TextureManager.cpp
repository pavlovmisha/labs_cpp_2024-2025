#include "TextureManager.h"
 SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	 SDL_Surface* tempSurface = IMG_Load(filename);
	 SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	 SDL_FreeSurface(tempSurface);
	 return texture;
}
