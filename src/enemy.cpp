#include "enemy.h"

//constructors
enemy::enemy()
	: character(), spriteIndex(0), hp(0) { }

enemy::enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
	: character(pos, size, sprite, color, velocity, renderer, game) {}

//animate the enemy by updating the sprite after a set interval
void enemy::updateSprite() {
	Sprite = sprites[(++spriteIndex) % sprites.size()];
}

bool enemy::updateState(float dt, glm::vec2 playerPos) {
	bool ret = false;

	fireUpdate += dt;
	spriteUpdateTime += dt;
	if (deathState == 0) {
		if (fireUpdate > fireDelay) { //determine if enemy should fire
			fireUpdate = 0.0f;
			ret = fire(playerPos);
		}
		if (spriteUpdateTime > spriteUpdateDelay) { //determine if sprite should update
			spriteUpdateTime = 0.0f;
			updateSprite();
		}
		move(dt, playerPos);   //move the enemy
	}
	else {  //if sprite is dying
		if (spriteUpdateTime > spriteUpdateDelay) {
			//update sprite for death animation
			spriteUpdateTime = 0.0f;
			if (spriteIndex != deathSprites.size() - 1) Sprite = deathSprites[(++spriteIndex)];

			else deathState = 2; //if last frame of the death animation is reached, change deathState to 2 to
			                     //indicate to the level that the enemy should be deleted
		}
	}
	moveBullets(dt);
	return ret;
}

void enemy::destroyBullet(int index) {
	delete bullets[index].bullet;
	bullets.erase(bullets.begin() + index);
}

void enemy::Draw() {
	//change color back to normal after taking damage
	if (glfwGetTime() - damageTime > 0.5) this->Color = glm::vec3(1.0f, 1.0f, 1.0f);

	renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}