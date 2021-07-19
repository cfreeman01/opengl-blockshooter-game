#pragma once
#include "enemy.h"
#include "trailGenerator.h"

//this enemy is spawned by the "enemy3" type and moves aggresively toward the player
class enemy3_mask : public enemy {
private:
	//movement
	void move(float dt, glm::vec2 playerPos);
	//trail
	trailGenerator* enemyTrail;
	//audio
	static audioPlayer damageAudio;
public:
	//constructors
	enemy3_mask();
	enemy3_mask(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);
	~enemy3_mask();

	static void loadTextures();

	//attack
	bool fire(glm::vec2 playerPos);
	virtual void moveBullets(float dt);

	//collisons
	void resolveCollision();

	void Draw();
};