#pragma once
#include "block.h"

//FORWARD DECLARATIONS
class Player;
class SpriteRenderer;

//a breakable block containing a powerup that allows the player to fire rapidly
class StarBlock : public Block
{
public:
	StarBlock();
	StarBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer);

	virtual int crack(Game* game);
	virtual void resolvePlayerCollision(Player &Player);
};