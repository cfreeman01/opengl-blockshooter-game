#pragma once
#include "blockObject.h"
#include "playerObject.h"
#include "resource_manager.h"

//a breakable block containing a powerup that allows the player to fire rapidly
class starBlock : public blockObject {

public:
	starBlock();
	starBlock(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer);

	virtual int crack(audioPlayer& levelAudio);
	virtual void resolvePlayerCollision(playerObject& Player);
};