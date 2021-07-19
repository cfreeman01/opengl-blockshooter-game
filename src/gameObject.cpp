#include "gameObject.h"

gameObject::gameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(){ }

gameObject::gameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite){ }

void gameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

std::vector<glm::vec2> gameObject::getVertices() {
	std::vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(Position.x, Position.y)); 
	vertices.push_back(glm::vec2(Position.x, Position.y + Size.y));
	vertices.push_back(glm::vec2(Position.x + Size.x, Position.y)); 
	vertices.push_back(glm::vec2(Position.x + Size.x, Position.y + Size.y));
	return vertices;
}