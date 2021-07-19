#include "trailGenerator.h"
#include <stdlib.h>

trailGenerator::trailGenerator(): object(nullptr), renderer(nullptr), lifeDecrease(0), updateRate(0) {}

trailGenerator::trailGenerator(gameObject& inObject, SpriteRenderer& inRenderer, float ld, float ur, std::vector<glm::vec4> colors):
	object(&inObject), renderer(&inRenderer), lifeDecrease(ld), updateRate(ur), colors(colors){}

void trailGenerator::init() {
	for (int i = 0; i < 10; i++) particles.push_back(particle());
}

//decrease particle life and add new particles
void trailGenerator::update(float dt) {
	updateTime += dt;
	if (updateTime < updateRate) return;
	updateTime = 0.0f;
	for (int i = 0; i < 10; i++)
		particles[i].life -= lifeDecrease;

	if (firstUnusedIndex() == -1) return;
	particles[firstUnusedIndex()] = particle(object->Position, colors[rand() % colors.size()], 0.5f, object->Rotation);
}

//render the particles
void trailGenerator::draw() {
	Shader sprite = ResourceManager::GetShader("sprite");
	for (int i = 9; i >= 0; i--) {
		if (particles[i].life > 0) {
			sprite.SetFloat("alpha", particles[i].life);
			renderer->DrawSprite(object->Sprite, particles[i].position, object->Size, particles[i].rotation, particles[i].color);
		}
	}
	sprite.SetFloat("alpha", 1.0f);
}

//find index of first 'dead' particle
int trailGenerator::firstUnusedIndex() {
	for (int i = 0; i < 10; i++) {
		if (particles[i].life <= 0.0f) return i;
	}
	return -1;
}