#pragma once
#include "enemy.h"

//this enemy moves directly toward the player and fires volleys of 3 bullets at a time
class enemy2 : public enemy {
private:
	//movement
	void move(float dt, glm::vec2 playerPos);
	//audio
	static audioPlayer shootAudio;
	static audioPlayer damageAudio;
public:
	//constructors
	enemy2();
	enemy2(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);

	static void loadTextures();

	//attack
	bool fire(glm::vec2 playerPos);

	//collisons
	void resolveCollision();
};