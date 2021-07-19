#include "healthBlock.h"

healthBlock::healthBlock()
	: blockObject() { }

healthBlock::healthBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer)
	: blockObject(pos, size, ResourceManager::GetTexture("healthBlock4"), color, velocity, renderer, false) {
	durability = 4;  //durability: 4:max, 3,2: cracked
	                 //1:block is broken and health can be picked up
	                 //0:block destroyed & health pickup gone
}

int healthBlock::crack(audioPlayer& levelAudio) {
		if (durability > 1) {  //if block is not broken
			if(durability == 2){ //if block is about to break
				levelAudio.play("audio/block_break.mp3");
				//spawn the health pickup
				Sprite = ResourceManager::GetTexture("heart");
				glm::vec2 newSize = glm::vec2(Size.x / 2.5, Size.y / 2.5);
				Position = glm::vec2(Position.x + Size.x/2 - newSize.x/2, Position.y + Size.y/2 - newSize.y/2);
				Size = newSize;
			}
			else {
				levelAudio.play("audio/block_crack.mp3");
				std::string newTexture = "healthBlock" + std::to_string(durability - 1);
				Sprite = ResourceManager::GetTexture(newTexture.c_str());
			}
			durability--;
		}
	return durability;
}

void healthBlock::resolvePlayerCollision(playerObject& Player) {
	if (durability == 1) {   //if health pickup is spawned
		Player.increaseHP(); //increase player's hp
		durability = 0;      //then set durability to 0 to indicate the object is destroyed
	}
	else Player.resolveCollision();  //otherwise just damage the player
}