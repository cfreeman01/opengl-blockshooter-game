#include "enemy3_mask.h"
#include "trailGenerator.h"
#include "spriteRenderer.h"
#include "resource_manager.h"
#include "game.h"
#include <GLFW/glfw3.h>

SoLoud::Wav Enemy3_Mask::damageAudio;
SoLoud::Wav Enemy3_Mask::deathAudio;

Enemy3_Mask::Enemy3_Mask()
	: Enemy() { }

Enemy3_Mask::Enemy3_Mask(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
	: Enemy(pos, size, sprite, color, velocity, renderer, game) {
	//textures must be loaded first
	sprites.push_back(ResourceManager::GetTexture("mask"));
	deathSprites.push_back(ResourceManager::GetTexture("maskdeath1"));
	deathSprites.push_back(ResourceManager::GetTexture("maskdeath2"));
	deathSprites.push_back(ResourceManager::GetTexture("maskdeath3"));
	spriteIndex = 0;
	spriteUpdateDelay = 0.1f;

	fireDelay = 5.0f;

	hp = 3;
	enemyTrail = new TrailGenerator(*this, renderer, 0.1f, 0.1f, std::vector<glm::vec4>(1, glm::vec4(0.694, 0.114, 0.114, 1.0)));
	enemyTrail->init();
}

Enemy3_Mask::~Enemy3_Mask() {
	delete enemyTrail;
}

void Enemy3_Mask::loadTextures() {
	ResourceManager::LoadTexture("textures/enemy3_mask.png", true, "mask");
	ResourceManager::LoadTexture("textures/enemy3_maskdeath1.png", true, "maskdeath1");
	ResourceManager::LoadTexture("textures/enemy3_maskdeath2.png", true, "maskdeath2");
	ResourceManager::LoadTexture("textures/enemy3_maskdeath3.png", true, "maskdeath3");
}

void Enemy3_Mask::loadAudio(){
	damageAudio.load("audio/enemy_hit.wav");
	deathAudio.load("audio/enemy_death.wav");
}

//this enemy does not fire projectiles, so these methods do nothing
bool Enemy3_Mask::fire(glm::vec2 playerPos) {
	return false;
}
void Enemy3_Mask::moveBullets(float dt) {

}

void Enemy3_Mask::move(float dt, glm::vec2 playerPos) { //move toward the player
	glm::vec2 diff = playerPos - this->Position;
	diff = glm::normalize(diff);
	diff = glm::length(Velocity) * diff;
	this->Position += diff * dt;
	enemyTrail->update(dt);
}

void Enemy3_Mask::resolveCollision() {
	hp--;
	damageTime = glfwGetTime();
	Color = glm::vec3(1.0f, 0.5f, 0.5f);  //change color to indicate damage
	if (hp == 0) {
		game->audioEngine->play(deathAudio);
		deathState = 1;  //change state to indicate the enemy is dying
		spriteIndex = 0; //and begin the death animation
		Sprite = deathSprites[0];
	}
	game->audioEngine->play(damageAudio);
}

void Enemy3_Mask::Draw() {
	//change color back to normal after taking damage
	if (glfwGetTime() - damageTime > 0.5) this->Color = glm::vec3(1.0f, 1.0f, 1.0f);

	enemyTrail->draw();
	renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}