#include "MenuItem.h"
#include <iostream>

using namespace std;

MenuItem::MenuItem() 
    : label(""), action(nullptr), position(0, 0), size(0, 0), 
      isSelected(false), renderer(nullptr), font(nullptr) {
    normalColor = Color(240, 240, 240, 255);
    selectedColor = Color(200, 220, 255, 255);
}

MenuItem::MenuItem(const std::string& label, std::function<void()> action,
                   SDL_Renderer* renderer, TTF_Font* font) 
    : label(label), action(action), position(0, 0), size(0, 0),
      isSelected(false), renderer(renderer), font(font) {
    normalColor = Color(240, 240, 240, 255);
    selectedColor = Color(200, 220, 255, 255);
}

void MenuItem::setPosition(const Point& position) {
    this->position = position;
}

void MenuItem::setSize(const Size& size) {
    this->size = size;
}

void MenuItem::setSelected(bool selected) {
    isSelected = selected;
}

void MenuItem::execute() const {
    if (action) {
        action();
    }
}

void MenuItem::render(SDL_Renderer* renderer) const {
    Color color = isSelected ? selectedColor : normalColor;
    
    SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
    SDL_Rect sdlRect = {position.getX(), position.getY(), size.getWidth(), size.getHeight()};
    SDL_RenderFillRect(renderer, &sdlRect);
    
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDrawLine(renderer, position.getX(), position.getY() + size.getHeight(), 
                       position.getX() + size.getWidth(), position.getY() + size.getHeight());
    
    if (font) {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, label.c_str(), textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                int textWidth = surface->w;
                int textHeight = surface->h;
                SDL_Rect textRect = {
                    position.getX() + 20,
                    position.getY() + (size.getHeight() - textHeight) / 2,
                    textWidth,
                    textHeight
                };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
}

bool MenuItem::containsPoint(const Point& point) const {
    return (point.getX() >= position.getX() && point.getX() <= position.getX() + size.getWidth() && 
            point.getY() >= position.getY() && point.getY() <= position.getY() + size.getHeight());
}

const std::string& MenuItem::getLabel() const {
    return label;
}