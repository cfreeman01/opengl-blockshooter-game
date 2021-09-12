#include "blockObject.h"
#include "resource_manager.h"
#include "playerObject.h"
#include <iostream>
#include <string>

SoLoud::Wav blockObject::crackAudio;
SoLoud::Wav blockObject::breakAudio;
SoLoud::Wav blockObject::unbreakableAudio;

blockObject::blockObject()
	: gameObject() {}

blockObject::blockObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, bool unbreakable)
	: gameObject(pos, size, sprite, color, velocity)
{
	durability = 3;
	this->unbreakable = unbreakable;
}

int blockObject::crack(Game *game)
{
	if (!unbreakable)
	{
		if (durability > 0)
		{
			durability--;
			if(durability > 0) game->audioEngine->play(crackAudio);
			else game->audioEngine->play(breakAudio);
		}

		//change texture to reflect new durability
		std::string newTexture = "block" + std::to_string(durability);
		Sprite = ResourceManager::GetTexture(newTexture.c_str());
	}
	else game->audioEngine->play(unbreakableAudio);
	return durability;
}

void blockObject::resolvePlayerCollision(playerObject &Player)
{
	Player.resolveCollision(); //for regular block, simply call the player's resolveCollision function
							   //for other block types that spawn pickups, this method has more functionality
}

void blockObject::loadTextures()
{
	ResourceManager::LoadTexture("textures/block_d3.png", true, "block3");
	ResourceManager::LoadTexture("textures/block_d2.png", true, "block2");
	ResourceManager::LoadTexture("textures/block_d1.png", true, "block1");
	ResourceManager::LoadTexture("textures/block_unbreakable.png", true, "blocku");
	ResourceManager::LoadTexture("textures/health_block1.png", true, "healthBlock2");
	ResourceManager::LoadTexture("textures/health_block2.png", true, "healthBlock3");
	ResourceManager::LoadTexture("textures/health_block3.png", true, "healthBlock4");
	ResourceManager::LoadTexture("textures/heart.png", true, "heart");
	ResourceManager::LoadTexture("textures/star_block1.png", true, "starBlock2");
	ResourceManager::LoadTexture("textures/star_block2.png", true, "starBlock3");
	ResourceManager::LoadTexture("textures/star_block3.png", true, "starBlock4");
	ResourceManager::LoadTexture("textures/star.png", true, "star");
}

void blockObject::loadAudio()
{
	crackAudio.load("audio/block_crack.wav");
	breakAudio.load("audio/block_break.wav");
	unbreakableAudio.load("audio/block_unbreakable.wav");
}