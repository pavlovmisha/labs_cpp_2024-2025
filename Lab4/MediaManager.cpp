#include "MediaManager.h"

#include <iostream>
#include <exception>

std::map<const char*, Mix_Chunk*> MediaManager::AudioItems;


class MediaException {};

void MediaManager::Init()
{
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout<<"SDL_mixer could not initialize! SDL_mixer Error: \n"<< Mix_GetError();
        throw new MediaException();
    }
}

void MediaManager::LoadAudio(const char* filename, const char* identifier, bool reload)
{
    // Only load if it is not already loaded unless reload = true
    if (AudioItems[identifier] == NULL || reload) {
        AudioItems[identifier] = Mix_LoadWAV(filename);
        if (AudioItems[identifier] == NULL) {
            std::cout << "Failed to load " << filename << "! SDL_mixer Error : \n" << Mix_GetError();
        }
    }
}

bool MediaManager::PlayAudioOnce(const char* identifier)
{
    if (AudioItems[identifier] != NULL) {
        Mix_PlayChannel(-1, AudioItems[identifier], 0);
        return true;
    }
    else {
        return false;
    }
}

void MediaManager::CleanUp()
{
    std::map<const char*, Mix_Chunk*>::iterator it = AudioItems.begin();
    while (it != AudioItems.end()) {
        Mix_FreeChunk(it->second);
        it++;
    }

    Mix_Quit();
}

// Cleanup