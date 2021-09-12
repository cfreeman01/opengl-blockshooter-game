#pragma once
#include "gameObject.h"
#include "trailGenerator.h"
#include "soloud.h"
#include "soloud_wav.h"
#include <vector>

//FORWARD DECLARATIONS
class Game;

//base class representing a player or enemy object
class character : public gameObject
{
public:
	struct Bullet
	{ //a data structure representing a single bullet along with its particle trail
		gameObject *bullet;
		trailGenerator trail;

		Bullet(gameObject *inBullet, SpriteRenderer *renderer, std::vector<glm::vec4> colors)
		{
			bullet = inBullet;
			trail = trailGenerator(*bullet, *renderer, 0.1f, 0.04f, colors);
		}
	};

protected:
	Game *game;
	SpriteRenderer *renderer;

	//bullets
	std::vector<Bullet> bullets; //currently active bullets fired by the character
	float fireUpdate = 0.0f;	 //elapsed time since last fire
	float bulletSpeed;
	float bulletSize;
	
public:
	//constructors/destructors
	character();
	character(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game);
	~character();

	//bullets
	virtual void moveBullets(float dt);
	void renderBullets();

	//collision detection
	std::vector<Bullet> &getBulletInfo();
	void destroyBullets(std::vector<bool> collisions);
};