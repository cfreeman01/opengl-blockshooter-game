#pragma once
#include "enemy.h"

//FORWARD DECLARATIONS
class TrailGenerator;

//this enemy is spawned by the "enemy3" type and moves aggresively toward the player
class Enemy3_Mask : public Enemy
{
private:
	//movement
	void move(float dt, glm::vec2 playerPos);
	//trail
	TrailGenerator *enemyTrail;

	//audio
	static SoLoud::Wav damageAudio;
	static SoLoud::Wav deathAudio;

public:
	//constructors
	Enemy3_Mask();
	Enemy3_Mask(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game);
	~Enemy3_Mask();

	static void loadTextures();
	static void loadAudio();

	//attack
	bool fire(glm::vec2 playerPos);
	virtual void moveBullets(float dt);

	//collisons
	void resolveCollision();

	void Draw();
};