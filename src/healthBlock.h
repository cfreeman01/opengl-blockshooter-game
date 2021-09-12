#pragma once
#include "block.h"

//FORWARD DECLARATIONS
class SpriteRenderer;
class Player;

//a breakable block that contains a health pickup
class HealthBlock : public Block {

public:
	HealthBlock();
	HealthBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer);

	virtual int crack(Game* game);
	virtual void resolvePlayerCollision(Player& Player);
};