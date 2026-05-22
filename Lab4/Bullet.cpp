#include "Bullet.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include <iostream>

SDL_Texture* Bullet::bulletTexture = nullptr;

class PlayerController;
SDL_Texture* Bullet::GetBulletTexture() {
	if (!Bullet::bulletTexture)
		Bullet::bulletTexture = TextureManager::LoadTexture("assets/bullet.png");

	return Bullet::bulletTexture;
}

Bullet::Bullet(float x, float y, float xDelta, float yDelta, BulletType type):GameObject("Bullet",Bullet::GetBulletTexture(), x, y) {
	this->type = type;
	this->xDelta = xDelta;
	this->yDelta = yDelta;
	GameObject::SetCollisionBox(textureWidth * textureUpscale, textureHeight * textureUpscale);
	GameObject::RegisterGameObject(this);
	std::cout << "Registered a bullet"<< this->type << std::endl;
}

void Bullet::Collided(GameObject* go) {
	EnemyController* enemy = dynamic_cast<EnemyController*>(go);
	if (type == BulletType::PLAYER_BULLET) {
		Game::player->weapon.AddBullet();
		if (enemy != NULL)
		{
			enemy->Destroy();
		}
		GameObject::DestroyGameObject((GameObject*)this);

	}
	else if (type == BulletType::ENEMY_BULLET) {
		if (enemy == NULL) {
			PlayerController* player = dynamic_cast<PlayerController*>(go);

			if (player != NULL) {
				player->TakeHit();
				std::cout << "Done!" << std::endl;
			}
			GameObject::DestroyGameObject((GameObject*)this);



		}
	}

}

void Bullet::Update() {
	x += xDelta * Game::deltaTime;
	y += yDelta * Game::deltaTime;

	if (y < 0 || y > Game::HEIGHT + 10.0f) {
		if (type == BulletType::PLAYER_BULLET) {
			Game::player->weapon.AddBullet();
		}
		GameObject::DestroyGameObject((GameObject*)this);
	}
	else {
		GameObject::Update();
	}
}