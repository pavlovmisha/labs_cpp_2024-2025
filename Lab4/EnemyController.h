#pragma once
#include "GameObject.h"
class Weapon;
class EnemyController : public GameObject
{
public:
    int enemyIndexX, enemyIndexY;
    Weapon* weapon;
    bool canShoot = false;

    EnemyController(const char* texturesheet, float x, float y);

    void Update();
    void Render();

    void Destroy();
    void AttemptShoot();
private:
    float movementSpeed;
    short int direction = 1;
    short int horizontalMovementCounter = 0;
    static const int horizontalMovementTriggerCount;
    static int timeDeductDelta;
    const float xStart;
    static const float xTravel;
    void move();

    Uint32 lastWeaponFire;
    Uint32 lastScoreDeductTick;

};


