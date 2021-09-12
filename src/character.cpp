#include "character.h"
#include "game.h"

Character::Character()
	: GameObject(), game(nullptr), renderer(nullptr) {}

Character::Character(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game)
	: GameObject(pos, size, sprite, color, velocity)
{
	this->renderer = &renderer;
	this->game = &game;
	bullets.reserve(500);
}

Character::~Character()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i].bullet;
	}
}

void Character::renderBullets()
{
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i].trail.draw();
		bullets[i].bullet->Draw(*renderer);
	}
}

void Character::moveBullets(float dt)
{
	fireUpdate += dt;
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i].bullet->Position += dt * bulletSpeed * bullets[i].bullet->Velocity;
		bullets[i].trail.update(dt);

		//check if bullet is out out of bounds
		if (bullets[i].bullet->Position.x > game->Width || bullets[i].bullet->Position.x < 0 || bullets[i].bullet->Position.y > game->playAreaHeight || bullets[i].bullet->Position.y < 0)
		{
			delete bullets[i].bullet;
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}

//COLLISION DETECTION
//return a reference to bullets to be used for collision detection
std::vector<Character::Bullet> &Character::getBulletInfo()
{
	return bullets;
}

//accepts a vector of booleans that indicates which bullets have collided with an object
//and destroys the appropriate bullets
void Character::destroyBullets(std::vector<bool> collisions)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (collisions[i])
		{
			delete bullets[i].bullet;
			bullets.erase(bullets.begin() + i);
		}
	}
}