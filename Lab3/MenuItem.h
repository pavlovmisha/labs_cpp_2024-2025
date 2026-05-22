#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Point.h"
#include "Size.h"
#include "Color.h"

class MenuItem {
private:
    std::string label;
    std::function<void()> action;
    Point position;
    Size size;
    bool isSelected;
    Color normalColor;
    Color selectedColor;
    SDL_Renderer* renderer;
    TTF_Font* font;

public:
    MenuItem();
    MenuItem(const std::string& label, std::function<void()> action, 
             SDL_Renderer* renderer, TTF_Font* font);
    
    void setPosition(const Point& position);
    void setSize(const Size& size);
    void setSelected(bool selected);
    void execute() const;
    void render(SDL_Renderer* renderer) const;
    
    bool containsPoint(const Point& point) const;
    const std::string& getLabel() const;
};

#endif