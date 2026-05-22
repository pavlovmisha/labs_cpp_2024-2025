#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <map>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "ActionHandler.h"
#include "MenuController.h"

int main(int argc, char* argv[]) {
    Renderer renderer(1024, 768, "Menu System");

    char* bp = SDL_GetBasePath();
    if (!bp) { SDL_Log("SDL_GetBasePath error: %s", SDL_GetError()); return 1; }
    std::string exeDir = bp;
    SDL_free(bp);

    std::string iniPath = exeDir + "resources/menu_config.txt";
    ResourceLoader loader;
    std::map<std::string, MenuDefinition> menus;
    if (!loader.loadFromFile(iniPath, menus)) {
        SDL_Log("Failed to load '%s'", iniPath.c_str());
        return 1;
    }

    bool running = true;
    int volume = 50;

    ActionHandler actionHandler;
    actionHandler.registerAction("exitApp", std::make_unique<ExitAppAction>(running));
    actionHandler.registerAction("startGame", std::make_unique<StartGameAction>());
    actionHandler.registerAction("increaseVolume", std::make_unique<IncreaseVolumeAction>(volume));
    actionHandler.registerAction("decreaseVolume", std::make_unique<DecreaseVolumeAction>(volume));
    actionHandler.registerAction("configureKeyboard", std::make_unique<ConfigureKeyboardAction>());
    actionHandler.registerAction("configureMouse", std::make_unique<ConfigureMouseAction>());

    MenuController menuController(renderer, menus, actionHandler, volume, "main");

    SDL_Event ev;
    while (running) {
        while (renderer.pollEvent(ev)) {
            if (ev.type == SDL_QUIT) running = false;
            else menuController.handleEvent(ev, running);
        }
        renderer.clear();
        menuController.render();
        renderer.present();
    }
    return 0;
}





