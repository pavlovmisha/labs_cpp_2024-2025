#pragma once
#include <vector>
#include "Bullet.h"

class Weapon
{
private:
	
	int usedBulletsCount = 0;
	int maxBullets;
	Bullet::BulletType bulletType;
public:
	Weapon();
	Weapon(int maxBullets);
	Weapon(int maxBullets, Bullet::BulletType type);
	

	bool Shoot(float x, float y, float xDelta, float yDelta);
	void AddBullet();
};
