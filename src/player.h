#pragma once
#include "character.h"
#include "spriteRenderer.h"
#include "soloud.h"
#include "soloud_wav.h"
#include <vector>

class Player : public Character
{
private:
	//hp and damage
	int hp;
	float damageCooldown = 1.0f;
	float damageTime = 0.0f;

	//powerup
	float powerupTime = 0.0f;

	//trail generator
	TrailGenerator *playerTrail;

	//bullets
	float fireDelay;
	std::vector<glm::vec4> bulletColors;

	//audio
	SoLoud::Wav shootAudio;
	SoLoud::Wav damageAudio;
	SoLoud::Wav powerupAudio;
	SoLoud::Wav healthupAudio;

public:
	//constructors/destructors
	Player();
	Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game);
	~Player();

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