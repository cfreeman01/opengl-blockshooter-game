#include "enemy2.h"

//static audio players for this enemy type
audioPlayer enemy2::shootAudio;
audioPlayer enemy2::damageAudio;

enemy2::enemy2()
	: enemy() { }

enemy2::enemy2(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
	: enemy(pos, size, sprite, color, velocity, renderer, game) {
	//textures must be loaded first
	sprites.push_back(ResourceManager::GetTexture("enemy2_1"));
	sprites.push_back(ResourceManager::GetTexture("enemy2_2"));
	sprites.push_back(ResourceManager::GetTexture("enemy2_3"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy2_death1"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy2_death2"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy2_death3"));
	spriteIndex = 0;
	spriteUpdateDelay = 0.25f;

	fireDelay = 5.0f;
	bulletColor = glm::vec4(0.392f, 0.924f, 0.698f, 1.0f);

	hp = 3;
	bulletSpeed = 500;
}

void enemy2::loadTextures() {
	ResourceManager::LoadTexture("textures/enemy2_1.png", true, "enemy2_1");
	ResourceManager::LoadTexture("textures/enemy2_2.png", true, "enemy2_2");
	ResourceManager::LoadTexture("textures/enemy2_3.png", true, "enemy2_3");
	ResourceManager::LoadTexture("textures/enemy2_death1.png", true, "enemy2_death1");
	ResourceManager::LoadTexture("textures/enemy2_death2.png", true, "enemy2_death2");
	ResourceManager::LoadTexture("textures/enemy2_death3.png", true, "enemy2_death3");
}

bool enemy2::fire(glm::vec2 playerPos) {
	shootAudio.play("audio/enemy2_gunshot.mp3");
	glm::vec2 diff = glm::normalize(playerPos - this->Position);
	glm::vec4 diff4 = glm::normalize(glm::vec4(diff, 1.0f, 1.0f));
	glm::mat4 rotate;

	//first bullet: follows player directly
	bullets.push_back(Bullet(new gameObject(this->Position, glm::vec2(10.0f, 10.0f),
		ResourceManager::GetTexture("bullet_circle"), bulletColor, diff),
		renderer, std::vector<glm::vec4>(1, bulletColor)));
	bullets.back().trail.init();

	//second bullet: rotated 25 degrees from diff
	rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	diff4 = rotate * diff4;
	bullets.push_back(Bullet(new gameObject(this->Position, glm::vec2(10.0f, 10.0f),
		ResourceManager::GetTexture("bullet_circle"), bulletColor, glm::normalize(glm::vec2(diff4.x, diff4.y))),
		renderer, std::vector<glm::vec4>(1, bulletColor)));
	bullets.back().trail.init();

	//third bullet: rotated -25 degrees from diff
	rotate = glm::mat4(1.0f);
	diff4 = glm::normalize(glm::vec4(diff, 1.0f, 1.0f));
	rotate = glm::rotate(rotate, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	diff4 = rotate * diff4;
	bullets.push_back(Bullet(new gameObject(this->Position, glm::vec2(10.0f, 10.0f),
		ResourceManager::GetTexture("bullet_circle"), bulletColor, glm::normalize(glm::vec2(diff4.x, diff4.y))),
		renderer, std::vector<glm::vec4>(1, bulletColor)));
	bullets.back().trail.init();

	if (bullets.size() == 500) bullets.erase(bullets.begin());
	bullets.back().bullet->Rotation = this->Rotation;
	return false;
}

void enemy2::move(float dt, glm::vec2 playerPos) { //if distance between enemy and player is >300px, move toward player
	glm::vec2 diff = playerPos - this->Position;
	if (glm::length(diff) < 300) return;
	diff = glm::normalize(diff);
	diff = glm::length(Velocity) * diff;
	this->Position += diff * dt;
}

void enemy2::resolveCollision() {
	damageAudio.play("audio/enemy_hit.mp3");
	hp--;
	damageTime = glfwGetTime();
	Color = glm::vec3(1.0f, 0.5f, 0.5f); //change enemy color to indicate damage
	if (hp == 0) {
		damageAudio.play("audio/enemy_death.mp3");
		deathState = 1;  //change state to indicate the enemy is dying
		spriteIndex = 0; //and begin the death animation
		Sprite = deathSprites[0];
	}
}