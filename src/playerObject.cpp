#include "playerObject.h"
#include "game.h"
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

audioPlayer playerObject::shootAudio;
audioPlayer playerObject::collisionAudio;


//CONSTRUCTORS
playerObject::playerObject()
    : character(), hp(3), fireDelay(0.5), playerTrail(nullptr) { }

playerObject::playerObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
    : character(pos, size, sprite, color, velocity, renderer, game) { 

	bulletColors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	bulletColors.push_back(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

	hp = 3;
	fireDelay = 0.5f;
	bulletSpeed = 1000;

	playerTrail = new trailGenerator(*this, renderer, 0.1f, 0.1f, bulletColors);
	playerTrail->init();
}

playerObject::~playerObject() {
	delete playerTrail;
}

//return vertices representing the hitbox for the player object
std::vector<glm::vec2> playerObject::getVertices() {
	std::vector<glm::vec2> vertices;
	vertices.push_back(glm::vec4(Position.x + Size.x / 2, Position.y, 0.0f, 1.0f));      //top vertex
	vertices.push_back(glm::vec4(Position.x, Position.y + Size.y, 0.0f, 1.0f));          //bottom left vertex
	vertices.push_back(glm::vec4(Position.x + Size.x, Position.y + Size.y, 0.0f, 1.0f)); //bottom right vertex
	return vertices;
}

//process keyboard input
void playerObject::move(float dt, glm::vec2 cursorPos) {
	//move player object with keys
	if (game->Keys[GLFW_KEY_A]) {
		if (Position.x >= 0)
			Position.x -= dt * Velocity.x;
	}
	if (game->Keys[GLFW_KEY_D]) {
		if (Position.x <= game->Width - Size.x)
			Position.x += dt * Velocity.x;
	}
	if (game->Keys[GLFW_KEY_W]) {
		if (Position.y >= 0)
			Position.y -= dt * Velocity.y;
	}
	if (game->Keys[GLFW_KEY_S]) {
		if (Position.y <= game->playAreaHeight - Size.y)
			Position.y += dt * Velocity.y;
	}

	//rotate player
	glm::vec2 diff = glm::normalize(Position - cursorPos);
	float angle = acos(glm::dot(diff, glm::vec2(0, 1)));
	if (cursorPos.x < Position.x) angle = -angle;
	Rotation = glm::degrees(angle);

	//update the player trail
	playerTrail->update(dt);
}

void playerObject::fire(glm::vec2 cursorPos, float dt) {
    if (fireUpdate < fireDelay) return;
	shootAudio.play("audio/gunshot.mp3");
	fireUpdate = 0.0;

    bullets.push_back(Bullet(new gameObject(this->Position, glm::vec2(10.0f, 10.0f), ResourceManager::GetTexture("bullet"),
		bulletColors[rand() % bulletColors.size()], glm::normalize(cursorPos - this->Position)), renderer, bulletColors));
	bullets.back().trail.init();

	if (bullets.size() == 500) bullets.erase(bullets.begin());
	bullets.back().bullet->Rotation = this->Rotation;
}

void playerObject::resolveCollision() {
	if (glfwGetTime() - damageTime > damageCooldown) {
		damageTime = glfwGetTime();
		collisionAudio.play("audio/take_damage.mp3");
		this->Color = glm::vec3(1.0f, 0.0f, 0.0f);  //change color to red to indicate damage
		hp--;
		if (hp == 0) {  //if player dies
			game->gameOver();
		}
	}
}
void playerObject::increaseHP() {
	collisionAudio.play("audio/hp_gain.mp3");
	if (hp < 3) hp++;
}
void playerObject::powerUp() {   //picking up a powerup decreases fireDelay,
	powerupTime = glfwGetTime(); //allowing the player to fire more rapidly
	collisionAudio.play("audio/powerup.mp3");
	fireDelay = 0.15f;
}

void playerObject::Draw() {
	float currentTime = glfwGetTime();
	//change color back to normal after taking damage
	if (currentTime - damageTime > damageCooldown) this->Color = glm::vec3(1.0f, 1.0f, 1.0f);
	//change fire rate back to normal after powerup
	if (currentTime - powerupTime > 5.5f) this->fireDelay = 0.5f;

	playerTrail->draw();
	renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

//reset player's state after game over
void playerObject::reset() {
	Position = glm::vec2(400.0f, 400.0f);
	bullets.clear();
	hp = 3;
}