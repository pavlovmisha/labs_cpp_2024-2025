#include "Input.h"

Uint8* Input::keystate;

void Input::SetKeystate() {
	keystate = const_cast <Uint8*>(SDL_GetKeyboardState(NULL));
}


bool Input::GetKeyDown(SDL_Scancode key) {
	return keystate[key];
}