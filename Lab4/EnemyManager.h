#pragma once
#include "EnemyController.h"
#include "Game.h"
#include <vector>

class EnemyManager
{
public:
	static int enemyShootingChance;
	static float enemyBulletSpeed;
	static int weaponFireFreezeTime;


	EnemyManager(int totalEnemies);
	
	void Init();
	void Update();

	void EnemyDead(EnemyController*);
	void PassEnemyShootingAbility(int indexX, int indexY);
private:
	static int maxInALayer;
	int enemyCount, totalEnemies;
	std::vector<EnemyController*> enemies;
	
};

