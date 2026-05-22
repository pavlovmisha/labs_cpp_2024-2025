#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Renderer {
    public:
        Renderer(int width, int height, const std::string& title);
        ~Renderer();
           
        void clear();        
        void present();
            
    void drawRect(int x, int y, int w, int h); 

    void fillRect(int x, int y, int w, int h);
    
    void drawText(const std::string& text, int x, int y);
   
    void getTextSize(const std::string& text, int& w, int& h);
    
    void getWindowSize(int& w, int& h);

        bool pollEvent(SDL_Event& ev);
    private:
        SDL_Window*   window_;
        SDL_Renderer* renderer_;
        TTF_Font*     font_;
    };
   