#include "Renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <stdexcept>

Renderer::Renderer(int w, int h, const std::string& title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("SDL_Init error: ") + SDL_GetError());
    }
    if (TTF_Init() < 0) {
        SDL_Quit();
        throw std::runtime_error(std::string("TTF_Init error: ") + TTF_GetError());
    }

    window_ = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!window_) {
        SDL_DestroyWindow(window_);
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateWindow error: ") + SDL_GetError());
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateRenderer error: ") + SDL_GetError());
    }

    char* basePath = SDL_GetBasePath();
    std::string fontPath = "resources/arial.ttf";
    SDL_free(basePath);

    font_ = TTF_OpenFont(fontPath.c_str(), 28);
    if (!font_) {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error(std::string("TTF_OpenFont failed: ") + TTF_GetError());
    }
}

void Renderer::getTextSize(const std::string& text, int& w, int& h) {
    if (TTF_SizeUTF8(font_, text.c_str(), &w, &h) != 0) {
        throw std::runtime_error(std::string("TTF_SizeUTF8 failed: ") + TTF_GetError());
    }
}

void Renderer::getWindowSize(int& w, int& h) {
    SDL_GetWindowSize(window_, &w, &h);
}

Renderer::~Renderer() {
    if (font_) TTF_CloseFont(font_);
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void Renderer::present() {
    SDL_RenderPresent(renderer_);
}

bool Renderer::pollEvent(SDL_Event& ev) {
    return SDL_PollEvent(&ev);
}

void Renderer::drawRect(int x, int y, int w, int h) {
    SDL_Rect rect{x, y, w, h};
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer_, &rect);
}

void Renderer::fillRect(int x, int y, int w, int h) {
    SDL_Rect rect{x, y, w, h};
    SDL_SetRenderDrawColor(renderer_, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::drawText(const std::string& text, int x, int y) {
    SDL_Color color{255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, text.c_str(), color);
    if (!surface) {
        throw std::runtime_error(std::string("TTF_RenderUTF8_Blended failed: ") + TTF_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_Rect dst{x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer_, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


