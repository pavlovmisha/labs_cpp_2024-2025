#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <functional>
#include "Menu.h"
#include "Dialog.h"
#include "ResourceLoader.h"
#include "Point.h"

using namespace std;

class MenuSystem {
private:
    unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
    unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;
    TTF_Font* font;
    
    map<string, Menu> menus;
    map<string, Dialog> dialogs;
    stack<Menu*> menuStack;
    Dialog* activeDialog;
    
    map<string, function<void()>> actions;
    
public:
    MenuSystem();
    ~MenuSystem();
    
    bool initialize();
    void loadResources(const string& filename);
    void run();
    
    void pushMenu(const string& menuName);
    void popMenu();
    void showDialog(const string& dialogName);
    
private:
    void handleEvents();
    void render();
    void setupActions();
};

#endif