#include "Menu.h"
#include <iostream>

using namespace std;

Menu::Menu() 
    : title(""), position(0, 0), size(0, 0), currentSelection(0), 
      renderer(nullptr), font(nullptr) {}

Menu::Menu(const string& title, const Point& position, const Size& size,
           SDL_Renderer* renderer, TTF_Font* font) 
    : title(title), position(position), size(size), 
      currentSelection(0), renderer(renderer), font(font) {}

void Menu::addItem(const MenuItem& item) {
    items.push_back(item);
    
    int headerHeight = size.getHeight() / 6;
    int availableHeight = size.getHeight() - headerHeight;
    int itemHeight = availableHeight / items.size();
    
    for (size_t i = 0; i < items.size(); i++) {
        items[i].setPosition(Point(position.getX(), position.getY() + headerHeight + i * itemHeight));
        items[i].setSize(Size(size.getWidth(), itemHeight));
    }
    
    if (items.size() == 1) {
        items[0].setSelected(true);
    }
}

void Menu::moveSelectionUp() {
    if (items.empty()) return;
    
    items[currentSelection].setSelected(false);
    currentSelection = (currentSelection - 1 + items.size()) % items.size();
    items[currentSelection].setSelected(true);
}

void Menu::moveSelectionDown() {
    if (items.empty()) return;
    
    items[currentSelection].setSelected(false);
    currentSelection = (currentSelection + 1) % items.size();
    items[currentSelection].setSelected(true);
}

void Menu::executeSelected() const {
    if (!items.empty()) {
        items[currentSelection].execute();
    }
}

void Menu::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_Rect menuRect = {position.getX(), position.getY(), size.getWidth(), size.getHeight()};
    SDL_RenderFillRect(renderer, &menuRect);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &menuRect);
    
    SDL_SetRenderDrawColor(renderer, 60, 120, 200, 255);
    int headerHeight = size.getHeight() / 6;
    SDL_Rect titleRect = {position.getX(), position.getY(), size.getWidth(), headerHeight};
    SDL_RenderFillRect(renderer, &titleRect);
    
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, title.c_str(), textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                int textWidth = surface->w;
                int textHeight = surface->h;
                SDL_Rect textRect = {
                    position.getX() + (size.getWidth() - textWidth) / 2,
                    position.getY() + (headerHeight - textHeight) / 2,
                    textWidth,
                    textHeight
                };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawLine(renderer, position.getX(), position.getY() + headerHeight, 
                       position.getX() + size.getWidth(), position.getY() + headerHeight);
    
    for (const auto& item : items) {
        item.render(renderer);
    }
}

void Menu::setPosition(const Point& position) {
    this->position = position;
}

void Menu::setSize(const Size& size) {
    this->size = size;
}

int Menu::getItemCount() const {
    return items.size();
}

bool Menu::handleMouseMove(const Point& point) {
    bool changed = false;
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].containsPoint(point)) {
            if (currentSelection != static_cast<int>(i)) {
                items[currentSelection].setSelected(false);
                currentSelection = i;
                items[currentSelection].setSelected(true);
                changed = true;
            }
            break;
        }
    }
    return changed;
}

bool Menu::handleMouseClick(const Point& point) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].containsPoint(point)) {
            items[i].execute();
            return true;
        }
    }
    return false;
}