
#include "MenuSystem.h"
#include <iostream>
#include <functional>
#include "ResourceLoader.h"

using namespace std;

MenuSystem::MenuSystem()
    : window(nullptr, SDL_DestroyWindow),
    renderer(nullptr, SDL_DestroyRenderer),
    font(nullptr),
    activeDialog(nullptr) {
    setupActions();
}

MenuSystem::~MenuSystem() {
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

bool MenuSystem::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! Error: " << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << endl;
        return false;
    }

    const char* fontPaths = "arial.ttf";
  
    font = TTF_OpenFont(fontPaths, 24);
    if (font) {
         std::cout << fontPaths << std::endl;
                
    }
     
    window.reset(SDL_CreateWindow("Menu System", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN));
    if (!window) {
        cerr << "Window could not be created! Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!renderer) {
        cerr << "Renderer could not be created! Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void MenuSystem::setupActions() {
    actions["exit"] = []() { exit(0); };
    actions["new_game"] = [this]() { showDialog("new_game"); };
    actions["open_settings"] = [this]() { pushMenu("settings"); };
    actions["back"] = [this]() { popMenu(); };
    actions["sound_settings"] = []() { cout << "Opening sound settings..." << endl; };
    actions["graphics_settings"] = []() { cout << "Opening graphics settings..." << endl; };
    actions["control_settings"] = []() { cout << "Opening control settings..." << endl; };
    actions["close_dialog"] = [this]() {
        if (activeDialog) activeDialog->hide();
    };
}

void MenuSystem::loadResources(const string& filename) {
    ResourceLoader loader(filename, actions);
    menus = loader.loadMenus(renderer.get(), font);
    dialogs = loader.loadDialogs(renderer.get(), font);

    if (!menus.empty()) {
        auto it = menus.find("main");
        if (it != menus.end()) {
            menuStack.push(&it->second);
            cout << "Main menu found and pushed to stack" << endl;
        }
        else {
            cerr << "Main menu not found!" << endl;
        }
    }
    else {
        cerr << "No menus loaded!" << endl;
    }
}

void MenuSystem::pushMenu(const string& menuName) {
    auto it = menus.find(menuName);
    if (it != menus.end()) {
        menuStack.push(&it->second);
    }
}

void MenuSystem::popMenu() {
    if (menuStack.size() > 1) {
        menuStack.pop();
    }
}

void MenuSystem::showDialog(const string& dialogName) {
    auto it = dialogs.find(dialogName);
    if (it != dialogs.end()) {
        activeDialog = &it->second;
        activeDialog->show();
    }
}

void MenuSystem::run() {
    bool quit = false;
    while (!quit) {
        handleEvents();
        render();
        SDL_Delay(16);
    }
}

void MenuSystem::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_MOUSEMOTION:
            if (activeDialog && activeDialog->getVisibility()) {
                activeDialog->handleMouseMove(Point(event.motion.x, event.motion.y));
            }
            else if (!menuStack.empty()) {
                menuStack.top()->handleMouseMove(Point(event.motion.x, event.motion.y));
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (activeDialog && activeDialog->getVisibility()) {
                    if (activeDialog->handleMouseDown(Point(event.button.x, event.button.y))) {
                        break;
                    }
                }

                if (!menuStack.empty()) {
                    menuStack.top()->handleMouseClick(Point(event.button.x, event.button.y));
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (activeDialog && activeDialog->getVisibility()) {
                    activeDialog->handleMouseUp();
                    activeDialog->handleClick(Point(event.button.x, event.button.y));
                }
            }
            break;
        }
    }
}

void MenuSystem::render() {
    SDL_SetRenderDrawColor(renderer.get(), 30, 30, 30, 255);
    SDL_RenderClear(renderer.get());

    if (!menuStack.empty()) {
        menuStack.top()->render(renderer.get());
    }

    if (activeDialog && activeDialog->getVisibility()) {
        activeDialog->render(renderer.get());
    }

    SDL_RenderPresent(renderer.get());
}