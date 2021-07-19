#include "enemy3.h"

//static audio players for this enemy type
audioPlayer enemy3::shootAudio;
audioPlayer enemy3::damageAudio;

enemy3::enemy3()
	: enemy() { }

enemy3::enemy3(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
	: enemy(pos, size, sprite, color, velocity, renderer, game) {
	sprites.push_back(ResourceManager::GetTexture("enemy3_1"));
	sprites.push_back(ResourceManager::GetTexture("enemy3_2"));
	sprites.push_back(ResourceManager::GetTexture("enemy3_3"));
	sprites.push_back(ResourceManager::GetTexture("enemy3_2"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy3_death1"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy3_death2"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy3_death3"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy3_death4"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy3_death5"));
	spriteIndex = 0;
	spriteUpdateDelay = 0.2f;

	fireDelay = 3.5f;
	bulletColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	hp = 3;
	bulletSpeed = 200;
}

void enemy3::loadTextures() {
	ResourceManager::LoadTexture("textures/enemy3_1.png", true, "enemy3_1");
	ResourceManager::LoadTexture("textures/enemy3_2.png", true, "enemy3_2");
	ResourceManager::LoadTexture("textures/enemy3_3.png", true, "enemy3_3");
	ResourceManager::LoadTexture("textures/enemy3_death1.png", true, "enemy3_death1");
	ResourceManager::LoadTexture("textures/enemy3_death2.png", true, "enemy3_death2");
	ResourceManager::LoadTexture("textures/enemy3_death3.png", true, "enemy3_death3");
	ResourceManager::LoadTexture("textures/enemy3_death4.png", true, "enemy3_death4");
	ResourceManager::LoadTexture("textures/enemy3_death5.png", true, "enemy3_death5");
}

bool enemy3::fire(glm::vec2 playerPos) {
	shootAudio.play("audio/enemy3_gunshot.mp3");
	return true;  //return true to indicate that the level should spawn a "mask" enemy
}
void enemy3::moveBullets(float dt) {

}

void enemy3::move(float dt, glm::vec2 playerPos) { //if distance between player and enemy is >300px, move toward player
	glm::vec2 diff = playerPos - this->Position;
	if (glm::length(diff) < 300) return;
	diff = glm::normalize(diff);
	diff = glm::length(Velocity) * diff;
	this->Position += diff * dt;
}

void enemy3::resolveCollision() {
	damageAudio.play("audio/enemy_hit.mp3");
	hp--;
	damageTime = glfwGetTime();
	Color = glm::vec3(1.0f, 0.5f, 0.5f); //change color to indicate damage
	if (hp == 0) {
		damageAudio.play("audio/enemy_death.mp3");
		deathState = 1;  //change state to indicate the enemy is dying
		spriteIndex = 0; //and begin the death animation
		Sprite = deathSprites[0];
	}
}