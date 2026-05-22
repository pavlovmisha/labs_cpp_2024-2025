// TrailPoint.cpp
#include "TrailPoint.h"
#include <cmath>

TrailPoint::TrailPoint() : radius(0.0f), alpha(0), creationTime(0) {}

TrailPoint::TrailPoint(const Vector2D& pos, float rad, Uint8 a, Uint32 time) 
    : position(pos), radius(rad), alpha(a), creationTime(time) {}

 Uint32 TrailPoint::getcreationTime() const { return creationTime; }
 //Vector2D TrailPoint::getposition() const { return position; }
 //float TrailPoint::getradius() const { return radius; }
 //Uint8 TrailPoint::getalpha() const { return alpha; }


void TrailPoint::update(float lifeRatio, float maxRadius) {
    radius = 3.0f + lifeRatio * (maxRadius - 3.0f);
    alpha = 100 - static_cast<Uint8>(lifeRatio * 100);
}

void TrailPoint::render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) const {
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);

    for (int dy = -radius; dy <= radius; dy++) {
        int dx = static_cast<int>(std::sqrt(radius * radius - dy * dy));
        SDL_RenderDrawLine(renderer,
            static_cast<int>(position.getX()) - dx,
            static_cast<int>(position.getY()) + dy,
            static_cast<int>(position.getX()) + dx,
            static_cast<int>(position.getY()) + dy);
    }
}