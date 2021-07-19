#pragma once
#include "enemy.h"

//this enemy acts as a spawner for the "enemy3_mask" enemy type
class enemy3 : public enemy {
private:
	//movement
	void move(float dt, glm::vec2 playerPos);
	//audio
	static audioPlayer shootAudio;
	static audioPlayer damageAudio;
public:
	//constructors
	enemy3();
	enemy3(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);

	static void loadTextures();

	//attack
	bool fire(glm::vec2 playerPos);
	virtual void moveBullets(float dt);

	//collisons
	void resolveCollision();
};