#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "MenuItem.h"
#include "Point.h"
#include "Size.h"

class Menu {
private:
    std::vector<MenuItem> items;
    int currentSelection;
    Point position;
    Size size;
    std::string title;
    SDL_Renderer* renderer;
    TTF_Font* font;

public:
    Menu();
    Menu(const std::string& title, const Point& position, const Size& size, 
         SDL_Renderer* renderer, TTF_Font* font);
    
    void addItem(const MenuItem& item);
    void moveSelectionUp();
    void moveSelectionDown();
    void executeSelected() const;
    void render(SDL_Renderer* renderer) const;
    
    void setPosition(const Point& position);
    void setSize(const Size& size);
    int getItemCount() const;
    
    bool handleMouseMove(const Point& point);
    bool handleMouseClick(const Point& point);
};

#endif