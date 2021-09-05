#pragma once
#include "resource_manager.h"
#include "shader.h"
#include "game.h"
#include "level.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class monkeyBoss : public gameObject
{
private:
	Game &game;
	SpriteRenderer &renderer;
	//rendering
	unsigned int VAO;
	Shader shader;
	Texture2D currentTexture;
	std::vector<Texture2D> deathTextures;
	//transformation matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	//movement
	float speed;
	bool initialOutOfBounds = true; //indicates that the boss is out of bounds after first spawning
	//attack
	float fireDelay = 3.3f;	 //delay between attacks
	float fireUpdate = 0.0f; //timer to keep track of attacks
	float bulletSpeed;
	glm::vec2 bulletSize = glm::vec2(game.Width/30.0f);
	float bombSpawnTime = 0.0f;			   //the time at which the boss spawns a bomb (explodes 1.5s later)
	std::vector<gameObject> bullets;	   //active bullets fired by the boss
	std::vector<bool> bombs;			   //boolean vector to indicate which items in "bullets" are bombs
	void spawnBanana(glm::vec2 vel);	   //spawn a regular banana (bullet)
	void spawnBananaBomb(glm::vec2 vel);   //spawn a bomb
	void explodeBananaBomb(glm::vec2 pos); //detonate a bomb
	//damage/hp
	int deathState = 0;			   //0:active, 1:dying, 2:dead
	float deathUpdateDelay = 0.3f; //delay between texture updates during death animation
	float deathUpdateTimer = 0.0f; //timer to track texture updates druing death animation
	int deathTextureIndex = 0;	   //index into deathTextures
	float damageTime = 0.0f;	   //time at which boss is damaged
	int hp = 40;
	//audio
	static audioPlayer shootAudio;
	static audioPlayer damageAudio;

public:
	//constructor
	monkeyBoss(Game &game, SpriteRenderer &renderer);
	//rendering
	void draw();
	std::vector<glm::vec2> getVertices(); //return vertices representing the hitbox of the boss
	static void loadTextures();
	//movement
	void move(float dt);
	void updateTransMatrices();
	//attack
	void fire(float dt, glm::vec2 playerPos);
	void moveBullets(float dt);
	int takeDamage();
	//damage/hp
	int getDeathState() { return deathState; }
	void updateDeathTexture(float dt);

	std::vector<gameObject> &getBulletInfo();
};