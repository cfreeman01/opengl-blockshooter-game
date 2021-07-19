#include "character.h"

character::character()
	: gameObject(), game(nullptr), renderer(nullptr){ }

character::character(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, SpriteRenderer& renderer, Game& game)
	: gameObject(pos, size, sprite, color, velocity) {
	this->renderer = &renderer;
	this->game = &game;
	bullets.reserve(500);
}

character::~character() {
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i].bullet;
	}
}

void character::renderBullets() {
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i].trail.draw();
		bullets[i].bullet->Draw(*renderer);
	}
}

void character::moveBullets(float dt) {
	fireUpdate += dt;
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i].bullet->Position += dt * bulletSpeed * bullets[i].bullet->Velocity;
		bullets[i].trail.update(dt);

		//check if bullet is out out of bounds
		if (bullets[i].bullet->Position.x > game->Width || bullets[i].bullet->Position.x < 0
			|| bullets[i].bullet->Position.y > game->playAreaHeight || bullets[i].bullet->Position.y < 0) {
			delete bullets[i].bullet;
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}

//COLLISION DETECTION
//return a reference to bullets to be used for collision detection
std::vector<character::Bullet>& character::getBulletInfo() {
	return bullets;
}

//accepts a vector of booleans that indicates which bullets have collided with an object
//and destroys the appropriate bullets
void character::destroyBullets(std::vector<bool> collisions) {
	for (int i = 0; i < bullets.size(); i++) {
		if (collisions[i]) {
			delete bullets[i].bullet;
			bullets.erase(bullets.begin() + i);
		}
	}
}