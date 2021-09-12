#pragma once
#include "enemy.h"

//this enemy moves directly toward the player and fires volleys of 3 bullets at a time
class Enemy2 : public Enemy
{
private:
	//movement
	void move(float dt, glm::vec2 playerPos);

	//audio
	static SoLoud::Wav damageAudio;
	static SoLoud::Wav deathAudio;
	static SoLoud::Wav shootAudio;

public:
	//constructors
	Enemy2();
	Enemy2(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game);

	static void loadTextures();
	static void loadAudio();

	//attack
	bool fire(glm::vec2 playerPos);

	//collisons
	void resolveCollision();
};