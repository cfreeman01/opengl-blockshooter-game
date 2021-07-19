#pragma once
#include "blockObject.h"
#include "playerObject.h"
#include "resource_manager.h"

//a breakable block that contains a health pickup
class healthBlock : public blockObject {

public:
	healthBlock();
	healthBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer);

	virtual int crack(audioPlayer& levelAudio);
	virtual void resolvePlayerCollision(playerObject& Player);
};