#include "Dialog.h"
#include <iostream>

using namespace std;

Dialog::Dialog() 
    : title(""), message(""), position(0, 0), size(0, 0), 
      isVisible(false), isDragging(false), dragOffsetX(0), dragOffsetY(0),
      renderer(nullptr), font(nullptr) {}

Dialog::Dialog(const string& title, const string& message, const Point& position, const Size& size,
               SDL_Renderer* renderer, TTF_Font* font) 
    : title(title), message(message), position(position), size(size), 
      isVisible(false), isDragging(false), dragOffsetX(0), dragOffsetY(0),
      renderer(renderer), font(font) {}

void Dialog::addButton(const string& label, function<void()> action) {
    buttons.push_back(make_pair(label, action));
}

void Dialog::show() {
    isVisible = true;
    isDragging = false;
}

void Dialog::hide() {
    isVisible = false;
    isDragging = false;
}

void Dialog::render(SDL_Renderer* renderer) const {
    if (!isVisible) return;
    
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_Rect dialogRect = {position.getX(), position.getY(), size.getWidth(), size.getHeight()};
    SDL_RenderFillRect(renderer, &dialogRect);
    
    SDL_SetRenderDrawColor(renderer, 60, 120, 200, 255);
    SDL_RenderDrawRect(renderer, &dialogRect);
    
    SDL_SetRenderDrawColor(renderer, 60, 120, 200, 255);
    SDL_Rect headerRect = {position.getX(), position.getY(), size.getWidth(), 40};
    SDL_RenderFillRect(renderer, &headerRect);
    
    if (font) {
        SDL_Color headerColor = {255, 255, 255, 255};
        SDL_Surface* headerSurface = TTF_RenderUTF8_Blended(font, title.c_str(), headerColor);
        if (headerSurface) {
            SDL_Texture* headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
            if (headerTexture) {
                SDL_Rect headerTextRect = {position.getX() + 10, position.getY() + 10, headerSurface->w, headerSurface->h};
                SDL_RenderCopy(renderer, headerTexture, NULL, &headerTextRect);
                SDL_DestroyTexture(headerTexture);
            }
            SDL_FreeSurface(headerSurface);
        }
    }
    
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDrawLine(renderer, position.getX(), position.getY() + size.getHeight() - 60, 
                       position.getX() + size.getWidth(), position.getY() + size.getHeight() - 60);
    
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = position.getY() + size.getHeight() - buttonHeight - 10;
    int totalButtonWidth = buttonWidth * buttons.size() + 20 * (buttons.size() - 1);
    int buttonStartX = position.getX() + (size.getWidth() - totalButtonWidth) / 2;
    
    for (size_t i = 0; i < buttons.size(); i++) {
        int buttonX = buttonStartX + i * (buttonWidth + 20);
        
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_Rect buttonRect = {buttonX, buttonY, buttonWidth, buttonHeight};
        SDL_RenderFillRect(renderer, &buttonRect);
        
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderDrawRect(renderer, &buttonRect);
        
        if (font) {
            SDL_Color textColor = {0, 0, 0, 255};
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font, buttons[i].first.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    int textWidth = surface->w;
                    int textHeight = surface->h;
                    SDL_Rect textRect = {
                        buttonX + (buttonWidth - textWidth) / 2,
                        buttonY + (buttonHeight - textHeight) / 2,
                        textWidth,
                        textHeight
                    };
                    SDL_RenderCopy(renderer, texture, NULL, &textRect);
                }
                SDL_FreeSurface(surface);
            }
        }
    }

    if (font && !message.empty()) {
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* messageSurface = TTF_RenderUTF8_Blended(font, message.c_str(), textColor);
        if (messageSurface) {
            SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
            if (messageTexture) {
                SDL_Rect messageRect = {position.getX() + 110, position.getY() + 50, messageSurface->w, messageSurface->h};
                SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
                SDL_DestroyTexture(messageTexture);
            }
            SDL_FreeSurface(messageSurface);
        }
    }
}

bool Dialog::handleMouseDown(const Point& point) {
    if (!isVisible) return false;
    
    SDL_Rect headerRect = {position.getX(), position.getY(), size.getWidth(), 40};
    if (point.getX() >= headerRect.x && point.getX() <= headerRect.x + headerRect.w &&
        point.getY() >= headerRect.y && point.getY() <= headerRect.y + headerRect.h) {
        isDragging = true;
        dragOffsetX = point.getX() - position.getX();
        dragOffsetY = point.getY() - position.getY();
        return true;
    }
    
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = position.getY() + size.getHeight() - buttonHeight - 10;
    int totalButtonWidth = buttonWidth * buttons.size() + 20 * (buttons.size() - 1);
    int buttonStartX = position.getX() + (size.getWidth() - totalButtonWidth) / 2;
    
    for (size_t i = 0; i < buttons.size(); i++) {
        int buttonX = buttonStartX + i * (buttonWidth + 20);
        
        if (point.getX() >= buttonX && point.getX() <= buttonX + buttonWidth &&
            point.getY() >= buttonY && point.getY() <= buttonY + buttonHeight) {
            return true;
        }
    }
    
    return false;
}

void Dialog::handleMouseMove(const Point& point) {
    if (isDragging) {
        int newX = point.getX() - dragOffsetX;
        int newY = point.getY() - dragOffsetY;
        
        int screenWidth, screenHeight;
        SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
        
        if (newX < 0) newX = 0;
        if (newY < 0) newY = 0;
        if (newX + size.getWidth() > screenWidth) newX = screenWidth - size.getWidth();
        if (newY + size.getHeight() > screenHeight) newY = screenHeight - size.getHeight();
        
        position = Point(newX, newY);
    }
}

void Dialog::handleMouseUp() {
    isDragging = false;
}

void Dialog::handleClick(const Point& point) const {
    if (!isVisible) return;
    
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = position.getY() + size.getHeight() - buttonHeight - 10;
    int totalButtonWidth = buttonWidth * buttons.size() + 20 * (buttons.size() - 1);
    int buttonStartX = position.getX() + (size.getWidth() - totalButtonWidth) / 2;
    
    for (size_t i = 0; i < buttons.size(); i++) {
        int buttonX = buttonStartX + i * (buttonWidth + 20);
        
        if (point.getX() >= buttonX && point.getX() <= buttonX + buttonWidth &&
            point.getY() >= buttonY && point.getY() <= buttonY + buttonHeight) {
            if (buttons[i].second) {
                buttons[i].second();
            }
            break;
        }
    }
}

bool Dialog::getVisibility() const {
    return isVisible;
}