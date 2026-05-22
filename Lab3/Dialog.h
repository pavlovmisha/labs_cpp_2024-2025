#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <vector>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Point.h"
#include "Size.h"

using namespace std;

class Dialog {
private:
    string title;
    string message;
    vector<pair<string, function<void()>>> buttons;
    Point position;
    Size size;
    bool isVisible;
    bool isDragging;
    int dragOffsetX, dragOffsetY;
    SDL_Renderer* renderer;
    TTF_Font* font;

public:
    Dialog();
    Dialog(const string& title, const string& message, const Point& position, const Size& size,
           SDL_Renderer* renderer, TTF_Font* font);
    
    void addButton(const string& label, function<void()> action);
    void show();
    void hide();
    void render(SDL_Renderer* renderer) const;
    bool handleMouseDown(const Point& point);
    void handleMouseMove(const Point& point);
    void handleMouseUp();
    void handleClick(const Point& point) const;
    
    bool getVisibility() const;
};

#endif