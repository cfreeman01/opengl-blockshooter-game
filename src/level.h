#pragma once
#include "blockObject.h"
#include "playerObject.h"
#include "healthBlock.h"
#include "starBlock.h"
#include "game.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy3_mask.h"
#include "audioPlayer.h"
#include <list>
#include <vector>

using namespace std;

class monkeyBoss;

class level {
private:
	SpriteRenderer* Renderer;
	Game& game;
	//environment obstacles
	std::list<std::list<blockObject*>> blocks;
	std::vector<enemy*> enemies;
	//block properties
	float speed;
	float blockSize;
	//enemies
	float enemySpawnDelay = 4.0f;  //delay between enemy spawns
	float enemySpawnTime = 0.0f;   //timer to determine when to spawn an enemt
	float bossSpawnTime = 120.0f;  //time, in seconds, at which the final boss spawns
	int enemyLevel = 1; //determines which types of enemies spawn, increases after set inteval
	monkeyBoss* finalBoss = nullptr;
public:
	//constructors/destructors
	level(SpriteRenderer& renderer, Game& game);
	~level();
	//render and draw blocks
	void addRow();
	void moveBlocks(float dt);
	void drawBlocks();
	//collision detection
	vector<bool> checkBulletsCollisions(vector<character::Bullet> bulletPositions);
	void checkPlayerCollisions(playerObject* Player);
	static bool checkCollisionSAT(gameObject& o1, gameObject& o2);
	static glm::vec2 proj(std::vector<glm::vec2> vertices, glm::vec2 axis);
	//enemies
	void updateEnemies(float dt, glm::vec2 playerPos);
	void renderEnemies();
	void increaseDifficulty();
};