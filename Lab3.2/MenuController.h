#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "ResourceLoader.h"
#include "ActionHandler.h"

class Renderer;

class MenuController {
public:
    MenuController(Renderer& renderer,
                   const std::map<std::string, MenuDefinition>& menus,
                   ActionHandler& actionHandler,
                   int& volume,
                   const std::string& startMenuId = "main");

    void handleEvent(const SDL_Event& ev, bool& running);
    void render();

private:
    Renderer& renderer_;
    const std::map<std::string, MenuDefinition>& menus_;
    ActionHandler& actionHandler_;
    int& volume_;
    std::vector<std::string> history_;
    std::string currentMenuId_;
    int selectedIndex_;
};


