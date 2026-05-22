#pragma once
#include "GameObject.h"
#include "TextureManager.h"


class Bullet : public GameObject {
	enum BulletType;
private:
	float xDelta, yDelta;
	static SDL_Texture* bulletTexture;
	BulletType type;
public:
	enum BulletType {
		PLAYER_BULLET,
		ENEMY_BULLET
	};
	Bullet(float x, float y, float xDelta, float yDelta, BulletType type);
	void Update();
	static SDL_Texture* GetBulletTexture();
	void Collided(GameObject* go) override;
};

