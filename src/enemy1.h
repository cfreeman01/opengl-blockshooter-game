#pragma once
#include "enemy.h"

//this is the most basic enemy that moves in a straight line and fires bullets directly at the player
class enemy1: public enemy {
private:
	//movement
	float finalXPosition;  //the final x position that the enemy will stay at until death
	void move(float dt, glm::vec2 playerPos);
	//audio
	static audioPlayer shootAudio;
	static audioPlayer damageAudio;

public:
	//constructors
	enemy1();
	enemy1(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);

	static void loadTextures();

	//attack
	bool fire(glm::vec2 playerPos);
	//collisons
	void resolveCollision();
};