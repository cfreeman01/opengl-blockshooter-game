#pragma once
#include "character.h"
#include "audioPlayer.h"
#include <vector>

class playerObject : public character {
private:
	//hp and damage
	int hp;
	float damageCooldown = 1.0f;
	float damageTime = 0.0f;

	//powerup
	float powerupTime = 0.0f;

	//trail generator
	trailGenerator* playerTrail;

	//bullets
	float fireDelay;
	std::vector<glm::vec4> bulletColors;

	//audio
	static audioPlayer shootAudio;
	static audioPlayer collisionAudio;
public:
	//constructors/destructors
	playerObject();
	playerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game);
	~playerObject();

	//accessors/mutators
	int getHP() { return hp; }
	//process keyboard input
	void move(float dt, glm::vec2 cursorPos);
	//bullets
	void fire(glm::vec2 cursorPos, float dt);
	//return three vertices of the player triangle object
	std::vector<glm::vec2> getVertices();
	//collisions/hp
	void resolveCollision();
	void increaseHP();
	//powerups
	void powerUp();
	//render
	void Draw();

	void reset();
};