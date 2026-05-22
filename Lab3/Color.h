#ifndef COLOR_H
#define COLOR_H
#include <SDL.h>

struct Color {
private:
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

public:
    Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
    
    Uint8 getR() const;
    Uint8 getG() const;
    Uint8 getB() const;
    Uint8 getA() const;
};

#endif