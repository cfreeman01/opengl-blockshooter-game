#pragma once
#include "character.h"
#include "soloud.h"
#include "soloud_wav.h"
#include <list>
#include <vector>

using namespace std;

//FORWARD DECLARATIONS
class Game;
class SpriteRenderer;
class Block;
class Enemy;
class MonkeyBoss;
class Player;

class level
{
private:
	SpriteRenderer *Renderer;
	Game &game;

	//environment obstacles
	std::list<std::list<Block *>> blocks;
	std::vector<Enemy *> enemies;

	//block properties
	float speed;
	float blockSize;

	//enemies
	float enemySpawnDelay = 4.0f; //delay between enemy spawns
	float enemySpawnTime = 0.0f;  //timer to determine when to spawn an enemt
	float bossSpawnTime = 120.0f; //time, in seconds, at which the final boss spawns
	int enemyLevel = 1;			  //determines which types of enemies spawn, increases after set inteval
	MonkeyBoss *finalBoss = nullptr;

	//audio
	SoLoud::Wav blockBreakAudio;
	SoLoud::Wav blockCrackAudio;
	SoLoud::Wav metalBlockAudio;

public:
	//constructors/destructors
	level(SpriteRenderer &renderer, Game &game);
	~level();
	//render and draw blocks
	void addRow();
	void moveBlocks(float dt);
	void drawBlocks();
	//collision detection
	vector<bool> checkBulletsCollisions(vector<Character::Bullet> bulletPositions);
	void checkPlayerCollisions(Player *currentPlayer);
	static bool checkCollisionSAT(GameObject &o1, GameObject &o2);
	static glm::vec2 proj(std::vector<glm::vec2> vertices, glm::vec2 axis);
	//enemies
	void updateEnemies(float dt, glm::vec2 playerPos);
	void renderEnemies();
	void increaseDifficulty();
};