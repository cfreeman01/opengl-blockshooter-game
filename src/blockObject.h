#pragma once
#include "gameObject.h"
#include "playerObject.h"
#include "resource_manager.h"

class blockObject : public gameObject {
protected:
	int durability; //3:max, 2,1:cracked, 0:broken
	bool unbreakable;
public:
	blockObject();
	blockObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, bool unbreakable=false);
	virtual int crack(audioPlayer& levelAudio);  //called when player shoots the block
	virtual void resolvePlayerCollision(playerObject& Player); //called when player collides with the block
	int getDurability() { return durability;  }
	static void loadTextures();
};