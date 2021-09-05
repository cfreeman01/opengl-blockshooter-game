#include "enemy1.h"

//static audio players for this enemy type
audioPlayer enemy1::shootAudio;
audioPlayer enemy1::damageAudio;

enemy1::enemy1()
	: enemy() {}

enemy1::enemy1(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game)
	: enemy(pos, size, sprite, color, velocity, renderer, game)
{
	//textures must be loaded first
	sprites.push_back(ResourceManager::GetTexture("enemy1_1"));
	sprites.push_back(ResourceManager::GetTexture("enemy1_2"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy1_death1"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy1_death2"));
	deathSprites.push_back(ResourceManager::GetTexture("enemy1_death3"));
	spriteIndex = 0;
	spriteUpdateDelay = 0.5f;

	fireDelay = 3.0f;
	bulletColor = glm::vec4(1.0f, 0.604f, 0.22f, 1.0f);

	finalXPosition = rand() % (game.Width - 40);
	hp = 3;
	bulletSpeed = game.Width/1.8f;
	bulletSize = game.Width/90.0f;
}

void enemy1::loadTextures()
{
	ResourceManager::LoadTexture("textures/enemy1_1.png", true, "enemy1_1");
	ResourceManager::LoadTexture("textures/enemy1_2.png", true, "enemy1_2");
	ResourceManager::LoadTexture("textures/enemy1_death1.png", true, "enemy1_death1");
	ResourceManager::LoadTexture("textures/enemy1_death2.png", true, "enemy1_death2");
	ResourceManager::LoadTexture("textures/enemy1_death3.png", true, "enemy1_death3");
}

bool enemy1::fire(glm::vec2 playerPos)
{
	shootAudio.play("audio/enemy1_gunshot.mp3");
	bullets.push_back(Bullet(new gameObject(this->Position, glm::vec2(bulletSize), //fire a bullet directly toward the player
											ResourceManager::GetTexture("bullet"), bulletColor, glm::normalize(playerPos - this->Position)),
							 renderer, std::vector<glm::vec4>(1, bulletColor)));
	bullets.back().trail.init();

	if (bullets.size() == 500)
		bullets.erase(bullets.begin());

	return false;
}

void enemy1::move(float dt, glm::vec2 playerPos)
{
	if (Position.x <= finalXPosition)
		return; //this enemy simply moves right until reaching its final x position
	Position.x -= Velocity.x * dt;
}

void enemy1::resolveCollision()
{ //called when player shoots the enemy
	damageAudio.play("audio/enemy_hit.mp3");
	hp--;
	damageTime = glfwGetTime();
	Color = glm::vec3(1.0f, 0.0f, 0.0f); //change enemy color to indicate damage
	if (hp == 0)
	{
		damageAudio.play("audio/enemy_death.mp3");
		deathState = 1;	 //change state to indicate the enemy is dying
		spriteIndex = 0; //and begin the death animation
		Sprite = deathSprites[0];
	}
}