#include "Text.h"
#include "TextManager.h"
#include "Game.h"


Text::Text(float x, float y, const char* text)
{
	// Register itself to TextManager
	TextManager::RegisterText(this);
	this->x = x;
	this->y = y;
	this->text = text;
	this->scaleX = 1;
	this->scaleY = 1;
	GenerateTexture();
}

void Text::GenerateTexture() {
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	if(surface != NULL)
		SDL_FreeSurface(surface);

	SDL_Color color = { 255,255,255,255 };
	

	this->surface = TTF_RenderText_Solid(TextManager::font, this->text, color);
	if (this->surface == NULL) {
		std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
	}

	this->texture = SDL_CreateTextureFromSurface(Game::renderer, surface);


}

void Text::ChangeText(const char* text)
{
	this->text = text;
	
	GenerateTexture();
}

void Text::render()
{
	SDL_Rect dest;
	dest.x = x - (surface->w * scaleX / 2); // made the pivot point to center
	dest.y = y - (surface->h * scaleY / 2);
	dest.w = surface->w * scaleX;
	dest.h = surface->h * scaleY;

	SDL_RenderCopy(Game::renderer, texture, NULL, &dest);
}

Text::~Text() {
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TextManager::UnregisterText(this);
}
