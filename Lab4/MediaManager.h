#pragma once
#include <map>
#include "SDL_mixer.h"

static class MediaManager
{
public:
	static void Init();
	static void LoadAudio(const char* filename, const char* identifier, bool reload = false);
	static bool PlayAudioOnce(const char* identifier);
	static void CleanUp();
private:
	static std::map<const char*, Mix_Chunk*> AudioItems;
};

