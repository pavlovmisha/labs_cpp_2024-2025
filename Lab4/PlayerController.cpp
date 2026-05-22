#include "PlayerController.h"
#include "TextManager.h"
#include "MediaManager.h"
#include "Text.h"

#include <string>


PlayerController::PlayerController(const char* texturesheet, float x, float y) :GameObject("Player",texturesheet, x, y, 14, 14, 3.0f) {
	movementSpeed = 400.0f;
	lastWeaponFire = SDL_GetTicks();
	weapon = Weapon(2, Bullet::BulletType::PLAYER_BULLET);
	GameObject::animated = true;
	GameObject::RegisterGameObject(this);
	hasCollision = true;
	SetCollisionBox(textureWidth * textureUpscale, textureHeight * textureUpscale);
	healthText = TextManager::AddText(100, Game::HEIGHT - 40, std::string("Health: ").append(std::to_string(health)).c_str());
}

PlayerController::~PlayerController()
{
	delete healthText;
}

int PlayerController::weaponFireFreezeTime = 500; //ms

 void PlayerController::Update() 
{
	 //// A simple statement to make the game lag
	 // for (int w = 10000000; w > 0; w--);
	 float xInput = 0;
	 if (Input::GetKeyDown(SDL_SCANCODE_A) || Input::GetKeyDown(SDL_SCANCODE_LEFT)) {
		xInput = -1;
	 }

	 if (Input::GetKeyDown(SDL_SCANCODE_D) || Input::GetKeyDown(SDL_SCANCODE_RIGHT)) {
		 xInput = 1;
	 }

	 if (Input::GetKeyDown(SDL_SCANCODE_SPACE) && (SDL_GetTicks() > lastWeaponFire + PlayerController::weaponFireFreezeTime)) {
		 //std::cout << " SPACE pressed" << std::endl;
		 bool shot = weapon.Shoot(x + (textureWidth * textureUpscale) / 3.5f, y - (textureHeight * textureUpscale)/2, 0.0f, -200.0f);
		 if (shot) {
			 lastWeaponFire = SDL_GetTicks();
			 MediaManager::PlayAudioOnce("laser_shooting");
		 }
	 }
	 move(xInput);
	 //weapon.Update();
	GameObject::Update();
	
}

 void PlayerController::TakeHit() {
	 health--;
	 std::cout << "Health: " << health << std::endl;
	 healthText->ChangeText(std::string("Health: ").append(std::to_string(health)).c_str());
	 MediaManager::PlayAudioOnce("damage");

	 // Update Health UI
	 if (health == 0) {
		 // Dead
		 Game::GameOver();

	 }
 }





 void PlayerController::move(float xInput) {
	 x += xInput * movementSpeed * Game::deltaTime;
	 
	 // substracting the actual width of texture since origin of the object is at top left
	 const float right_limit = Game::WIDTH - GameObject::textureWidth * GameObject::textureUpscale; 

	 if (x >= right_limit) { 
		 x = right_limit;
	 }
	 else if (x <= 0) {
		 x = 0;
	 }
 }
