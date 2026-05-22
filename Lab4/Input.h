#pragma once
#include "SDL.h"
#include <iostream>
static class Input
{
public:
	static Uint8* keystate;
	static bool GetKeyDown(SDL_Scancode key);

	static void SetKeystate();
};

