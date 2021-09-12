#include "player.h"
#include "game.h"
#include "spriteRenderer.h"
#include "resource_manager.h"
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

//CONSTRUCTORS
Player::Player()
	: Character(), hp(3), fireDelay(0.5), playerTrail(nullptr) {}

Player::Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer &renderer, Game &game)
	: Character(pos, size, sprite, color, velocity, renderer, game)
{

	bulletColors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

	hp = 3;
	fireDelay = 0.5f;
	bulletSpeed = game.Width / 0.9f;
	bulletSize = game.Width / 90.0f;

	playerTrail = new TrailGenerator(*this, renderer, 0.1f, 0.1f, bulletColors);
	playerTrail->init();

	shootAudio.load("audio/gunshot.wav");
	damageAudio.load("audio/take_damage.wav");
	powerupAudio.load("audio/powerup.wav");
	healthupAudio.load("audio/hp_gain.wav");
}

Player::~Player()
{
	delete playerTrail;
}

//return vertices representing the hitbox for the player object
std::vector<glm::vec2> Player::getVertices()
{
	std::vector<glm::vec2> vertices;
	vertices.push_back(glm::vec4(Position.x + Size.x / 2, Position.y, 0.0f, 1.0f));		 //top vertex
	vertices.push_back(glm::vec4(Position.x, Position.y + Size.y, 0.0f, 1.0f));			 //bottom left vertex
	vertices.push_back(glm::vec4(Position.x + Size.x, Position.y + Size.y, 0.0f, 1.0f)); //bottom right vertex
	return vertices;
}

//process keyboard input
void Player::move(float dt, glm::vec2 cursorPos)
{
	//move player object with keys
	if (game->Keys[GLFW_KEY_A])
	{
		if (Position.x >= 0)
			Position.x -= dt * Velocity.x;
	}
	if (game->Keys[GLFW_KEY_D])
	{
		if (Position.x <= game->Width - Size.x)
			Position.x += dt * Velocity.x;
	}
	if (game->Keys[GLFW_KEY_W])
	{
		if (Position.y >= 0)
			Position.y -= dt * Velocity.y;
	}
	if (game->Keys[GLFW_KEY_S])
	{
		if (Position.y <= game->playAreaHeight - Size.y)
			Position.y += dt * Velocity.y;
	}

	//rotate player
	glm::vec2 diff = glm::normalize(Position - cursorPos);
	float angle = acos(glm::dot(diff, glm::vec2(0, 1)));
	if (cursorPos.x < Position.x)
		angle = -angle;
	Rotation = glm::degrees(angle);

	//update the player trail
	playerTrail->update(dt);
}

void Player::fire(glm::vec2 cursorPos, float dt)
{
	if (fireUpdate < fireDelay)
		return;
	fireUpdate = 0.0;

	SoLoud::handle h = game->audioEngine->play(shootAudio);

	bullets.push_back(Bullet(new GameObject(this->Position, glm::vec2(bulletSize), ResourceManager::GetTexture("bullet"),
											bulletColors[rand() % bulletColors.size()], glm::normalize(cursorPos - this->Position)),
							 renderer, bulletColors));
	bullets.back().trail.init();

	if (bullets.size() == 500)
		bullets.erase(bullets.begin());
	bullets.back().bullet->Rotation = this->Rotation;
}

void Player::resolveCollision()
{
	if (glfwGetTime() - damageTime > damageCooldown)
	{
		damageTime = glfwGetTime();
		this->Color = glm::vec3(1.0f, 0.0f, 0.0f); //change color to red to indicate damage
		hp--;
		game->audioEngine->play(damageAudio);
		if (hp == 0)
		{ //if player dies
			game->gameOver();
		}
	}
}
void Player::increaseHP()
{
	if (hp < 3)
		hp++;
	game->audioEngine->play(healthupAudio);
}
void Player::powerUp()
{								 //picking up a powerup decreases fireDelay,
	powerupTime = glfwGetTime(); //allowing the player to fire more rapidly
	fireDelay = 0.15f;
	game->audioEngine->play(powerupAudio);
}

void Player::Draw()
{
	float currentTime = glfwGetTime();
	//change color back to normal after taking damage
	if (currentTime - damageTime > damageCooldown)
		this->Color = glm::vec3(1.0f, 1.0f, 1.0f);
	//change fire rate back to normal after powerup
	if (currentTime - powerupTime > 5.5f)
		this->fireDelay = 0.5f;

	playerTrail->draw();
	renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

//reset player's state after game over
void Player::reset()
{
	Position = glm::vec2(400.0f, 400.0f);
	bullets.clear();
	hp = 3;
}