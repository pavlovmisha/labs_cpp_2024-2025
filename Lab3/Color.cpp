#include "Color.h"

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    : r(r), g(g), b(b), a(a) {}

Uint8 Color::getR() const { return r; }
Uint8 Color::getG() const { return g; }
Uint8 Color::getB() const { return b; }
Uint8 Color::getA() const { return a; }