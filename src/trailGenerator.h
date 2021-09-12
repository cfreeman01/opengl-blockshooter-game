#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

//FORWARD DECLARATIONS
class gameObject;
class SpriteRenderer;

//represents one trailing object
struct particle
{
	glm::vec2 position;
	glm::vec4 color;
	float life;
	float rotation;

	particle()
		: position(0.0f), color(1.0f), life(0.0f), rotation(0.0f) {}
	particle(glm::vec2 pos, glm::vec4 col, float lif, float rot)
		: position(pos), color(col), life(lif), rotation(rot) {}
};

//used to generate a trail behind a moving object
class trailGenerator
{
private:
	gameObject *object; //source object
	SpriteRenderer *renderer;
	std::vector<particle> particles;
	int firstUnusedIndex();	 //finds index of first 'dead' particle
	float updateTime = 0.0f; //keeps track of time since last update
	float lifeDecrease;		 //amount of decrease in particle life per update
	float updateRate;
	std::vector<glm::vec4> colors; //colors used to render the particles
public:
	trailGenerator();
	trailGenerator(gameObject &inObject, SpriteRenderer &inRenderer, float ld, float ur, std::vector<glm::vec4> colors);
	void init();
	void update(float dt);
	void draw();
};