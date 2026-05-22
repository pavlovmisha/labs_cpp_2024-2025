#include "GameObject.h"

 std::vector<GameObject*> GameObject::gameobjects;
 int GameObject::lastId = 0;

 std::vector<GameObject::GameObjectWithDelay> GameObject::gameobjectsToDelete;

GameObject::GameObject(const char* name, const char* texturesheet, float x, float y, int textureWidth, int textureHeight, float textureUpscale) {
	objectTexture = TextureManager::LoadTexture(texturesheet);
	this->name = (char*)name;
	this->x = x;
	this->y = y;
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
	this->textureUpscale = textureUpscale;

	this->animParams.animationStartTick = SDL_GetTicks();
}

GameObject::GameObject(const char* name, SDL_Texture* texture, float x, float y, int textureWidth, int textureHeight, float textureUpscale) {
	objectTexture = texture;
	this->name = (char*)name;
	this->x = x;
	this->y = y;
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
	this->textureUpscale = textureUpscale;

	this->animParams.animationStartTick = SDL_GetTicks();

}



void GameObject::Update() {
	

	
	srcRect.w = textureWidth;
	srcRect.h = textureHeight;
	if (animated) {
		if (animParams.animationLoop) {
			srcRect.x = textureWidth * static_cast<int>((SDL_GetTicks() - animParams.animationStartTick) / (animParams.animationDelay) % animParams.animationFrames);
		}
		else {
			int frame = static_cast<int>((SDL_GetTicks() - animParams.animationStartTick) / (animParams.animationDelay));
			srcRect.x = textureWidth * (frame < animParams.animationFrames ? frame: animParams.animationFrames - 1);
		}
	}
	else {
		srcRect.x = 0;

	}
	srcRect.y = animParams.animationIndex * textureHeight;

	destRect.w = srcRect.w * textureUpscale;
	destRect.h = srcRect.h * textureUpscale;
	destRect.x = x;
	destRect.y = y;

	if (hasCollision) {
		UpdateCollisionBox();
		CheckForCollision();
	}
}

void GameObject::CheckForCollision() {
	for (int i = 0; i < gameobjects.size(); i++) {
		if (gameobjects.at(i)->hasCollision && gameobjects.at(i) != this)  {

			if (Collision::AABB(collisionBox, gameobjects.at(i)->collisionBox)) {
				gameobjects.at(i)->Collided(this);
			}
		}
	}
}

void GameObject::SetCollisionBox( int w, int h)
{
	hasCollision = true;
	collisionBox.x = this->x;
	collisionBox.y = this->y;
	collisionBox.w = w;
	collisionBox.h = h;
}

void GameObject::UpdateCollisionBox()
{
	collisionBox.x = this->x;
	collisionBox.y = this->y;
}

void GameObject::ChangeAnimation(short int index)
{
	animParams.animationIndex = index;
}

void GameObject::ChangeAnimationParams(GameObject::AnimationParams newParams) {
	animParams = newParams;
}



void GameObject::Collided(GameObject* go)
{
	std::cout << name << " Collide with " << go->name << std::endl;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objectTexture, &srcRect, &destRect);
	if (Game::displayGizmos) {
		if (hasCollision) {
			SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(Game::renderer, &collisionBox);
		}
	}
}

int GameObject::RegisterGameObject(GameObject* go)
{
	go->ID = ++lastId;
	gameobjects.push_back(go);
	return go->ID;
}

void GameObject::DestroyGameObject(GameObject* go)
{
	std::vector<GameObject*>::iterator it = std::find(gameobjects.begin(), gameobjects.end(), go);
	if (it != gameobjects.end()) {
		gameobjects.erase(std::remove(gameobjects.begin(), gameobjects.end(), go), gameobjects.end());
		delete go;
	}
}

void GameObject::DestroyGameObject(GameObject* go, int delay)
{
	GameObjectWithDelay gwd;
	gwd.go = go;
	gwd.tickToDelete = SDL_GetTicks() + delay;
	gameobjectsToDelete.push_back(gwd);
}

void GameObject::UpdateEverything()
{
	if (Game::gamePaused) return;
	
	std::vector<GameObject::GameObjectWithDelay>::iterator it = gameobjectsToDelete.begin();
		
	while (it < gameobjectsToDelete.end()) {
		if (SDL_GetTicks() >= it->tickToDelete) {
			GameObject* temp = it->go;
			it = gameobjectsToDelete.erase(it);
			DestroyGameObject(temp);
		}
		else {
			it++;
		}
	}

	for (int i = 0; i < gameobjects.size(); i++) {
		gameobjects.at(i)->Update();
	}
}


void GameObject::RenderEverything()
{
	for (int i = 0; i < gameobjects.size(); i++) {
		gameobjects.at(i)->Render();
	}
}

GameObject::~GameObject() {
	
}
