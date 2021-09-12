#pragma once
#include "blockObject.h"

//FORWARD DECLARATIONS
class SpriteRenderer;
class playerObject;

//a breakable block that contains a health pickup
class healthBlock : public blockObject {

public:
	healthBlock();
	healthBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer);

	virtual int crack(Game* game);
	virtual void resolvePlayerCollision(playerObject& Player);
};