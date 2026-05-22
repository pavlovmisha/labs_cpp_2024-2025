// TrailPoint.h
#pragma once
#include "Vector2D.h"
#include <SDL.h>

class TrailPoint {
private:
    Vector2D position;
    float radius;
    Uint8 alpha;
    Uint32 creationTime;

public:
    TrailPoint();
    TrailPoint(const Vector2D& pos, float rad, Uint8 a, Uint32 time);
    
    Uint32 getcreationTime() const;
    //Vector2D getposition() const;
    //float getradius() const;
    //Uint8 getalpha() const;
    
    void update(float lifeRatio, float maxRadius);

    //friend class Game; 

    void render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) const;
};