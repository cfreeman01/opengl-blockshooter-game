#include "level.h"
#include "monkeyBoss.h"
#include "block.h"
#include "player.h"
#include "healthBlock.h"
#include "starBlock.h"
#include "game.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy3_mask.h"
#include "resource_manager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

glm::vec4 blockColors[] = { //possible colors for blocks
	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
	glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
	glm::vec4(1.0f, 0.0f, 0.5f, 1.0f),
	glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
	glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)};

level::level(SpriteRenderer &renderer, Game &game) : game(game)
{
	Renderer = &renderer;
	speed = 30.0f;
	blockSize = (float)game.playAreaHeight / 15;

	blockCrackAudio.load("audio/block_crack.wav");
	blockBreakAudio.load("audio/block_break.wav");
	metalBlockAudio.load("audio/block_unbreakable.wav");
}

level::~level()
{
	for (auto row = blocks.begin(); row != blocks.end(); row++)
	{
		for (auto block = row->begin(); block != row->end(); block++)
		{
			delete *block;
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
	}
}

//RENDER AND MOVE BLOCKS-------------
void level::addRow()
{
	if (game.elapsedTime >= (bossSpawnTime - 10))
		return;
	srand(time(0));
	blocks.push_front(std::list<Block *>());
	for (int i = 0; i < 15; i++)
	{

		if (rand() % 100 > 65)
		{ //add normal or unbreakable block
			if (rand() % 100 > 15)
			{ //normal
				blocks.front().push_back(new Block(glm::vec2(game.Width + blockSize, i * blockSize),
														 glm::vec2(blockSize, blockSize),
														 ResourceManager::GetTexture("block3"),
														 blockColors[rand() % 6],
														 glm::vec2(0.0f, 0.0f), *Renderer, false));
			}
			else
			{ //unbreakable
				blocks.front().push_back(new Block(glm::vec2(game.Width + blockSize, i * blockSize),
														 glm::vec2(blockSize),
														 ResourceManager::GetTexture("blocku"),
														 glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
														 glm::vec2(0.0f, 0.0f), *Renderer, true));
			}
		}
		else if (rand() % 200 >= 199)
		{ //add health block
			blocks.front().push_back(new HealthBlock(glm::vec2(game.Width + blockSize, i * blockSize),
													 glm::vec2(blockSize),
													 glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
													 glm::vec2(0.0f, 0.0f), *Renderer));
		}
		else if (rand() % 400 >= 399)
		{ //add star block
			blocks.front().push_back(new StarBlock(glm::vec2(game.Width + blockSize, i * blockSize),
												   glm::vec2(blockSize),
												   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
												   glm::vec2(0.0f, 0.0f), *Renderer));
		}
	}
	if (blocks.front().empty())
	{ //each row should have at least one block
		blocks.front().push_back(new Block(glm::vec2(game.Width + blockSize, rand() % 15 * blockSize),
												 glm::vec2(blockSize, blockSize),
												 ResourceManager::GetTexture("block3"),
												 blockColors[rand() % 6],
												 glm::vec2(0.0f, 0.0f), *Renderer, false));
	}
}

void level::moveBlocks(float dt)
{
	if (blocks.empty())
		return;
	//move blocks
	for (auto row = blocks.begin(); row != blocks.end(); row++)
	{
		for (auto block = row->begin(); block != row->end(); block++)
		{
			(*block)->Position.x -= dt * speed;
		}
	}
	//spawn in new row if needed
	auto rightRow = blocks.begin();
	if ((*rightRow->begin())->Position.x < game.Width - blockSize)
	{
		addRow();
	}
	//delete row if needed
	auto leftRow = --blocks.end();
	if (leftRow->empty() || (*leftRow->begin())->Position.x < -blockSize)
	{
		for (auto itr = leftRow->begin(); itr != leftRow->end(); itr++)
		{
			delete *itr;
		}
		blocks.erase(leftRow);
	}
}

void level::drawBlocks()
{
	for (auto row = blocks.rbegin(); row != blocks.rend(); row++)
	{
		for (auto block = row->rbegin(); block != row->rend(); block++)
		{
			(*block)->Draw(*Renderer);
		}
	}
}
//---------------------------------------

//ENEMIES-------------------------------
void level::updateEnemies(float dt, glm::vec2 playerPos)
{
	//update or remove existing enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->deathState == 2)
		{ //if enemy is dead, remove it from the list
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
			enemySpawnTime -= 2.0; //and turn back the enemy spawn timer
			continue;
		}
		bool enemy3fired = enemies[i]->updateState(dt, playerPos); //indicates whether an "enemy3" type has fired
		if (enemy3fired)
		{ //if so, spawn a new "mask" enemy
			glm::vec2 size     = glm::vec2(game.Width/22.5f, game.Width/28.0f);
			glm::vec2 velocity = glm::vec2(game.Width/6.0f);
			enemies.push_back(new Enemy3_Mask(enemies[i]->Position + glm::vec2(enemies[i]->Size.x / 2, enemies[i]->Size.y / 2),
											  size, ResourceManager::GetTexture("mask"), glm::vec3(1.0f, 1.0f, 1.0f),
											  velocity, *Renderer, game));
		}
	}
	//possibly spawn a new enemy
	if (finalBoss == nullptr && game.State == GAME_ACTIVE)
	{
		enemySpawnTime += dt;
		if (enemySpawnTime > enemySpawnDelay && enemies.size() < 5)
		{
			if (rand() % 100 > 50)
			{
				enemySpawnTime = 0.0f;

				int enemyType = (rand() % enemyLevel) + 1;
				if (enemyType == 1) //spawn enemy1
				{
					glm::vec2 size     = glm::vec2(game.Width/22.5f);
					glm::vec2 velocity = glm::vec2(game.Width/9.0f);
					enemies.push_back(new Enemy1(glm::vec2(game.Width + size.x, size.y + rand() % (game.playAreaHeight - 2*(int)size.y)),
												 size, ResourceManager::GetTexture("enemy1_1"), glm::vec3(1.0f, 1.0f, 1.0f),
												 velocity, *Renderer, game));
				}
				else if (enemyType == 2) //spawn enemy2
				{
					glm::vec2 size     = glm::vec2(game.Width/22.5f, game.Width/30);
					glm::vec2 velocity = glm::vec2(game.Width/12.0f);
					enemies.push_back(new Enemy2(glm::vec2(game.Width + size.x, size.y + rand() % (game.playAreaHeight - 2*(int)size.y)),
												 size, ResourceManager::GetTexture("enemy2_1"), glm::vec3(1.0f, 1.0f, 1.0f),
												 velocity, *Renderer, game));
				}
				else if (enemyType == 3) //spawn enemy3
				{
					glm::vec2 size     = glm::vec2(game.Width/16.6f, game.Width/18);
					glm::vec2 velocity = glm::vec2(game.Width/15.0f);
					enemies.push_back(new Enemy3(glm::vec2(game.Width + size.x, size.y + rand() % (game.playAreaHeight - 2*(int)size.y)),
												 size, ResourceManager::GetTexture("enemy3_1"), glm::vec3(1.0f, 1.0f, 1.0f),
												 velocity, *Renderer, game));
				}
			}
			else
				enemySpawnTime -= 1.0;
		}
	}
	//update boss
	else if (finalBoss != nullptr)
	{

		if (finalBoss->getDeathState() == 2)
		{					  //if boss is dead,
			delete finalBoss; //delete it
			finalBoss = nullptr;

			//increase the boss spawn time and resume the regular level
			bossSpawnTime += (120.0f + (game.elapsedTime - bossSpawnTime));
			addRow();
		}

		else
		{
			if (finalBoss->getDeathState() == 0)
			{ //if boss is active
				finalBoss->move(dt);
				finalBoss->fire(dt, playerPos);
			}
			if (finalBoss->getDeathState() == 1)
			{ //if boss is dying
				finalBoss->updateDeathTexture(dt);
			}
			finalBoss->moveBullets(dt);
		}
	}
	//possibly spawn boss
	if (game.elapsedTime >= bossSpawnTime && finalBoss == nullptr)
	{
		finalBoss = new MonkeyBoss(game, *Renderer);
	}
}
void level::renderEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Draw();
		enemies[i]->renderBullets();
	}
	if (finalBoss != nullptr)
		finalBoss->draw();
}
//---------------------------------------

void level::increaseDifficulty()
{
	//every minute for first two minutes, add new enemy types
	if (game.elapsedTime >= 30 && enemyLevel == 1)
	{
		enemyLevel++;
	}
	if (game.elapsedTime >= 60 && enemyLevel == 2)
	{
		enemyLevel++;
	}
	//increase speed of blocks
	speed += game.Width / 90.0f;
}

//COLLISION DETECTION

//checks if blocks and enemies have collided with the player's bullets
//returns an array of booleans to denote which bullets have been collided with
std::vector<bool> level::checkBulletsCollisions(std::vector<Character::Bullet> bulletInfo)
{
	std::vector<bool> collisions(bulletInfo.size(), false);

	//check block collisions
	for (auto row = blocks.begin(); row != blocks.end(); row++)
	{
		for (auto block = row->begin(); block != row->end(); block++)
		{
			for (int i = 0; i < bulletInfo.size(); i++)
			{
				if (checkCollisionSAT(**block, *(bulletInfo[i].bullet)))
				{
					collisions[i] = true;
					(*block)->crack(&game);
				}
			}
			if ((*block)->getDurability() == 0) //if block is destroyed
			{						   
				delete *block;			   //delete it
				block = row->erase(block); //and erase it from the list
				if (block == row->end())
					break;
			}
		}
		if (row->empty())
			row = blocks.erase(row); //if entire row is empty, remove it
		if (row == blocks.end())
			break;
	}

	//check enemy collisions
	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < bulletInfo.size(); j++)
		{
			if (checkCollisionSAT(*enemies[i], *(bulletInfo[j].bullet)) && enemies[i]->deathState == 0)
			{
				collisions[j] = true;
				enemies[i]->resolveCollision();
			}
		}
	}

	//check boss collisions
	if (finalBoss != nullptr)
	{
		for (int j = 0; j < bulletInfo.size(); j++)
		{
			if (checkCollisionSAT(*finalBoss, *(bulletInfo[j].bullet)))
			{
				collisions[j] = true;
				finalBoss->takeDamage();
			}
		}
	}
	return collisions;
}

//check player collision with blocks and enemies
void level::checkPlayerCollisions(Player *Player)
{
	//check player/block collisions
	for (auto row = blocks.rbegin(); row != blocks.rend(); row++)
	{
		for (auto block = row->rbegin(); block != row->rend(); block++)
		{
			if (checkCollisionSAT(*Player, **block))
			{
				(*block)->resolvePlayerCollision(*Player);
				return;
			}
		}
	}

	//check player/enemy and player/enemybullet collisions
	std::vector<Character::Bullet> enemyBullets;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (checkCollisionSAT(*Player, *enemies[i]) && enemies[i]->deathState == 0)
		{
			enemies[i]->deathState = 2; //if player collides with enemy, destroy the enemy
			Player->resolveCollision();
			return;
		}
		//check if player collided with enemy bullets
		enemyBullets = enemies[i]->getBulletInfo();
		for (int j = 0; j < enemyBullets.size(); j++)
			if (checkCollisionSAT(*Player, *enemyBullets[j].bullet))
			{
				enemies[i]->destroyBullet(j);
				Player->resolveCollision();
				return;
			}
	}

	//check player/boss collision
	if (finalBoss != nullptr)
	{
		if (checkCollisionSAT(*Player, *finalBoss))
		{
			Player->resolveCollision();
			return;
		}
		std::vector<GameObject> &bossBullets = finalBoss->getBulletInfo();
		for (int i = 0; i < bossBullets.size(); i++)
		{
			if (checkCollisionSAT(*Player, bossBullets[i]))
			{
				Player->resolveCollision();
				return;
			}
		}
	}
}

//check collision of two game objects by separating axis theorem
bool level::checkCollisionSAT(GameObject &o1, GameObject &o2)
{
	//get vertices of the objects
	std::vector<glm::vec2> v1 = o1.getVertices();
	std::vector<glm::vec2> v2 = o2.getVertices();

	//get edges of both objects
	std::vector<glm::vec2> edges1;
	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v1.size(); j++)
		{
			if (i != j)
				edges1.push_back(v1[i] - v1[j]);
		}
	}
	std::vector<glm::vec2> edges2;
	for (int i = 0; i < v2.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			if (i != j)
				edges2.push_back(v2[i] - v2[j]);
		}
	}
	//calculate normals from edges
	std::vector<glm::vec2> normals;
	for (int i = 0; i < edges1.size(); i++)
	{
		normals.push_back(glm::vec2(-edges1[i].y, edges1[i].x));
	}
	for (int i = 0; i < edges2.size(); i++)
	{
		normals.push_back(glm::vec2(-edges2[i].y, edges2[i].x));
	}

	for (int i = 0; i < normals.size(); i++)
	{
		//get projections
		glm::vec2 proj1 = proj(v1, normals[i]);
		glm::vec2 proj2 = proj(v2, normals[i]);

		//test if they overlap
		if ((proj1.x >= proj2.y) || (proj2.x >= proj1.y))
		{
			return false;
		}
	}
	return true;
}

glm::vec2 level::proj(std::vector<glm::vec2> vertices, glm::vec2 axis)
{
	float min, max; //min and max lengths of projections of vertices onto axis
	min = max = glm::dot(vertices[0], axis);
	for (int i = 1; i < vertices.size(); i++)
	{
		float result = glm::dot(vertices[i], axis);
		if (result < min)
			min = result;
		if (result > max)
			max = result;
	}
	return glm::vec2(min, max); //return the min and max values in a vector
}