#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "texture.h"
#include "spriteRenderer.h"
class gameObject {
public:
    // object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    gameObject();
    gameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);

    virtual std::vector<glm::vec2> getVertices();
};