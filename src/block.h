#pragma once
#include "game.h"
#include "gameObject.h"

//FORWARD DECLARATIONS
class Player;

class Block : public GameObject
{
protected:
	int durability; //3:max, 2,1:cracked, 0:broken
	bool unbreakable;

	//audio
	static SoLoud::Wav crackAudio;
	static SoLoud::Wav breakAudio;
	static SoLoud::Wav unbreakableAudio;

public:
	Block();
	Block(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, bool unbreakable = false);
	virtual int crack(Game *game);							   //called when player shoots the block
	virtual void resolvePlayerCollision(Player &Player); //called when player collides with the block
	int getDurability() { return durability; }
	bool isUnbreakable() { return unbreakable; }
	static void loadTextures();
	static void loadAudio();
};