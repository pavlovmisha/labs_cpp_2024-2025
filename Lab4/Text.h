#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

class Text
{
public:
	float x, y, scaleX, scaleY;

	Text(float x, float y, const char* text);
	void GenerateTexture();
	~Text();

	void ChangeText(const char* text);
	void render();

	void SetScale(float x, float y) { scaleX = x; scaleY = y; };
private:
	const char* text;
	SDL_Surface* surface;
	SDL_Texture* texture;
};

