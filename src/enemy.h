#pragma once
#include "character.h"

//abstract class that all enemy types inherit from
class Enemy : public Character
{
protected:
	//sprites
	std::vector<Texture2D> sprites;		 //sprites for the enemy's regular animation
	std::vector<Texture2D> deathSprites; //sprites for the enemy's death animation
	int spriteIndex;					 //index into sprites or deathSprites
	float spriteUpdateDelay;			 //duration of each frame of animation
	float spriteUpdateTime = 0.0f;		 //keeps track of elapsed time for animations

	//bullets
	float fireDelay; //time between attacks
	glm::vec4 bulletColor;

	//movement
	virtual void move(float dt, glm::vec2 playerPos) = 0;

	//collisions
	int hp;
	float damageTime = 0.0f; //time at which enemy was last damaged

public:
	bool updateState(float dt, glm::vec2 playerPos); //update the enemy's state each frame

	//constructors
	Enemy();
	Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game);

	//sprites
	void updateSprite();
	virtual void Draw();

	//bullets
	void destroyBullet(int index);
	virtual bool fire(glm::vec2 playerPos) = 0; //fire a bullet. only returns true when the "enemy3" type fires,
												//in order to indicate to the level to spawn a new enemy
												//(enemy3 acts like a spawner)
	//collisions
	virtual void resolveCollision() = 0;
	int deathState = 0; //0:alive, 1:dying, 2:dead
};