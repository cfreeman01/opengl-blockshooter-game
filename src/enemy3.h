#pragma once
#include "enemy.h"

//this enemy acts as a spawner for the "enemy3_mask" enemy type
class Enemy3 : public Enemy {
private:
	//movement
	void move(float dt, glm::vec2 playerPos);

	//audio
	static SoLoud::Wav damageAudio;
	static SoLoud::Wav deathAudio;
	static SoLoud::Wav shootAudio;

public:
	//constructors
	Enemy3();
	Enemy3(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);

	static void loadTextures();
	static void loadAudio();

	//attack
	bool fire(glm::vec2 playerPos);
	virtual void moveBullets(float dt);

	//collisons
	void resolveCollision();
};