#pragma once
#include "game.h"
#include "gameObject.h"

//FORWARD DECLARATIONS
class playerObject;

class blockObject : public gameObject
{
protected:
	int durability; //3:max, 2,1:cracked, 0:broken
	bool unbreakable;

	//audio
	static SoLoud::Wav crackAudio;
	static SoLoud::Wav breakAudio;
	static SoLoud::Wav unbreakableAudio;

public:
	blockObject();
	blockObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, bool unbreakable = false);
	virtual int crack(Game* game);							   //called when player shoots the block
	virtual void resolvePlayerCollision(playerObject &Player); //called when player collides with the block
	int getDurability() { return durability; }
	bool isUnbreakable() { return unbreakable; }
	static void loadTextures();
	static void loadAudio();
};