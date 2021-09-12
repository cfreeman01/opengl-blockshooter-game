#include "monkeyBoss.h"
#include "resource_manager.h"
#include "shader.h"
#include "game.h"
#include "level.h"

float vertices[] = { //cube vertices
    -0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
-0.1f,  0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,

-0.1f, -0.1f,  0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,
-0.1f, -0.1f,  0.1f,

-0.1f,  0.1f,  0.1f,
-0.1f,  0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,
-0.1f, -0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,

 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,

-0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f, -0.1f,  0.1f,
-0.1f, -0.1f,  0.1f,
-0.1f, -0.1f, -0.1f,

-0.1f,  0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,
-0.1f,  0.1f, -0.1f,
};
float textureCoords[] = {  //texture coordinates
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,

 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,

 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,

 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,

 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f,

 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f
};
unsigned int indices[] = {
0, 1, 3, // first triangle
1, 2, 3  // second triangle
};

void MonkeyBoss::loadTextures()
{
    ResourceManager::LoadTexture("textures/discomonke.png", false, "discomonke");

    ResourceManager::LoadTexture("textures/discomonke_death1.png", true, "discomonke_death1");
    ResourceManager::LoadTexture("textures/discomonke_death2.png", true, "discomonke_death2");
    ResourceManager::LoadTexture("textures/discomonke_death3.png", true, "discomonke_death3");
    ResourceManager::LoadTexture("textures/discomonke_death4.png", true, "discomonke_death4");
    ResourceManager::LoadTexture("textures/discomonke_death5.png", true, "discomonke_death5");
    ResourceManager::LoadTexture("textures/discomonke_death6.png", true, "discomonke_death6");
    ResourceManager::LoadTexture("textures/discomonke_death7.png", true, "discomonke_death7");
    ResourceManager::LoadTexture("textures/discomonke_death8.png", true, "discomonke_death8");
    ResourceManager::LoadTexture("textures/discomonke_death9.png", true, "discomonke_death9");

    ResourceManager::LoadTexture("textures/banana1.png", true, "banana1");
    ResourceManager::LoadTexture("textures/banana3.png", true, "banana3");
}

//construct and initialize
MonkeyBoss::MonkeyBoss(Game &game, SpriteRenderer &renderer) : game(game), renderer(renderer)
{
    Position = glm::vec2(game.Width + 500.0f, -500.0f);
    Velocity = glm::vec2(-1.0f, 1.0f);
    speed = game.Width/3;
    bulletSpeed = game.Width/1.6f;

    shader = ResourceManager::LoadShader("shaders/bossVertexShader.vert", "shaders/bossFragmentShader.frag", nullptr, "boss");
    currentTexture = ResourceManager::GetTexture("discomonke");

    deathTextures.reserve(9);
    for (int i = 1; i <= 9; i++)
    {
        deathTextures.push_back(ResourceManager::GetTexture(("discomonke_death" + to_string(i)).c_str()));
    }

    //initialize audio
    shootAudio.load("audio/boss_fire.wav");
    damageAudio.load("audio/boss_hit.wav");
    deathAudio.load("audio/boss_death.wav");
    bombAudio.load("audio/boss_bananabreak.wav");

    //Initialize opengl objects
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int pointsEBO;
    glGenBuffers(1, &pointsEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointsEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int textureEBO;
    glGenBuffers(1, &textureEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    shader.Use();
    shader.SetInteger("bossTexture", 0);
    shader.SetVector3f("color", glm::vec3(1.0f, 1.0f, 1.0f));
    glBindVertexArray(0);

    updateTransMatrices();
}

//render the boss and bullets
void MonkeyBoss::draw()
{
    shader.Use();
    updateTransMatrices();
    currentTexture.Bind();
    float time = glfwGetTime();

    //set color back to normal after taking damage
    if (time - damageTime >= 0.4f && damageTime != 0.0f)
        shader.SetVector3f("color", glm::vec3(1.0f, 1.0f, 1.0f));

    //draw cube
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);

    //draw bullets
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i].Draw(renderer);
    }
}

//movement
void MonkeyBoss::move(float dt)
{
    Position += speed * dt * glm::normalize(Velocity);

    if (!initialOutOfBounds)
    { //if boss is not intially out of bounds after spawning,
        if (Position.x <= 0)
            Velocity.x = -Velocity.x; //then the boss should "bounce" off the edges of the window
        if (Position.x >= game.Width)
            Velocity.x = -Velocity.x;
        if (Position.y <= 0)
            Velocity.y = -Velocity.y;
        if (Position.y >= game.playAreaHeight)
            Velocity.y = -Velocity.y;
    }
    else
    { //if boss enters the bounds of the window after being initially out of bounds, set initialOutOfBounds to false
        if (Position.x > 0 && Position.x < game.Width && Position.y > 0 && Position.y < game.Height)
            initialOutOfBounds = false;
    }
}

//return vertices representing the (approximate) collision box for the boss
std::vector<glm::vec2> MonkeyBoss::getVertices()
{
    std::vector<glm::vec4> bossVertices; //vector of all boss vertices
    std::vector<glm::vec2> retVertices;  //vector of "outer" vertices to return
    for (int i = 0; i < 36; i++)
    {
        bossVertices.push_back(glm::vec4(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f)); //get coordinates of vertices
    }

    float maxX = -1000000;
    float minX = 1000000;
    float maxY = -1000000;
    float minY = 1000000;

    for (int i = 0; i < bossVertices.size(); i++)
    {
        bossVertices[i] = model * bossVertices[i]; //move the vertices to boss' current position

        bossVertices[i].x *= game.Width;   //change the coordinate system of the vertices
        bossVertices[i].y *= -game.Height; //to make x fall between 0 and game width, and y between 0 and game height
        bossVertices[i].x += (float)game.Width / 2;
        bossVertices[i].y += (float)game.Height / 2;

        if (bossVertices[i].x < minX)
            minX = bossVertices[i].x; //keep track of max and min x and y values
        if (bossVertices[i].x > maxX)
            maxX = bossVertices[i].x;
        if (bossVertices[i].y < minY)
            minY = bossVertices[i].y;
        if (bossVertices[i].y > maxY)
            maxY = bossVertices[i].y;
    }

    //here we approximate a collision box for the boss by returning all vertices that have a max or min x/y value
    //this is an approximation of the "outer" vertices of the cube object when all the vertices of the 3d object are projected in a 2d plane.
    bool cont;
    for (int i = 0; i < bossVertices.size(); i++)
    {
        cont = false;
        if (bossVertices[i].x == maxX || bossVertices[i].y == maxY || bossVertices[i].x == minX || bossVertices[i].y == minY)
        {
            for (int j = 0; j < retVertices.size(); j++)
            {
                if (retVertices[j] == glm::vec2(bossVertices[i]))
                    cont = true;
            }
            if (cont)
                continue;
            retVertices.push_back(glm::vec2(bossVertices[i]));
        }
    }
    return retVertices;
}

//BULLET METHODS------------
void MonkeyBoss::fire(float dt, glm::vec2 playerPos)
{
    fireUpdate += dt;
    if (fireUpdate < fireDelay)
        return;
    fireUpdate = 0.0f;
    game.audioEngine->play(shootAudio);

    glm::vec2 diff = glm::normalize(playerPos - this->Position);
    glm::vec4 diff4 = glm::normalize(glm::vec4(diff, 1.0f, 1.0f));
    glm::mat4 rotate;

    //first bullet: follows player directly
    if (rand() % 15 < 10)
        spawnBanana(diff);
    else
        spawnBananaBomb(diff);

    //second bullet: rotated 45 degrees from diff
    rotate = glm::mat4(1.0f);
    rotate = glm::rotate(rotate, glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    diff4 = rotate * diff4;
    if (rand() % 15 < 10)
        spawnBanana(glm::normalize(glm::vec2(diff4)));
    else
        spawnBananaBomb(glm::normalize(glm::vec2(diff4)));

    //third bullet: rotated -45 degrees from diff
    rotate = glm::mat4(1.0f);
    diff4 = glm::normalize(glm::vec4(diff, 1.0f, 1.0f));
    rotate = glm::rotate(rotate, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    diff4 = rotate * diff4;
    if (rand() % 15 < 10)
        spawnBanana(glm::normalize(glm::vec2(diff4)));
    else
        spawnBananaBomb(glm::normalize(glm::vec2(diff4)));
}

void MonkeyBoss::spawnBanana(glm::vec2 vel)
{
    bullets.push_back(GameObject(this->Position, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), vel));
    bombs.push_back(false);
}

void MonkeyBoss::spawnBananaBomb(glm::vec2 vel)
{
    bullets.push_back(GameObject(this->Position, bulletSize,
                                 ResourceManager::GetTexture("banana3"), glm::vec3(1.0f, 1.0f, 1.0f), vel));
    bombs.push_back(true);
}

void MonkeyBoss::explodeBananaBomb(glm::vec2 pos)
{
    game.audioEngine->play(bombAudio);

    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.707f, 0.0f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.707f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(-0.707f, 0.0f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, -0.707f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(-0.5f, 0.5f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, -0.5f)));
    bullets.push_back(GameObject(pos, bulletSize,
                                 ResourceManager::GetTexture("banana1"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(-0.5f, -0.5f)));
    for (int i = 0; i < 8; i++)
    {
        bombs.push_back(false);
    }
}

void MonkeyBoss::moveBullets(float dt)
{
    fireUpdate += dt;
    for (unsigned int i = 0; i < bullets.size(); i++)
    {
        bullets[i].Position += dt * bulletSpeed * bullets[i].Velocity;

        if (bombs[i] && fireUpdate >= 1.5f)
        {                                           //if bullet is a bomb and more than 1.5s have passed since bomb has spawned,
            explodeBananaBomb(bullets[i].Position); //then detonate the bomb
            bullets.erase(bullets.begin() + i);     //amd then erase it
            bombs.erase(bombs.begin() + i);
            i--;
            continue;
        }

        //check if bullet is out out of bounds
        if (bullets[i].Position.x > game.Width || bullets[i].Position.x < 0 || bullets[i].Position.y > game.playAreaHeight || bullets[i].Position.y < 0)
        {
            if (bombs[i])
            {
                explodeBananaBomb(bullets[i].Position);
            }
            bullets.erase(bullets.begin() + i);
            bombs.erase(bombs.begin() + i);
            i--;
        }
    }
}
//--------------------------------

int MonkeyBoss::takeDamage()
{
    hp--;
    game.audioEngine->play(damageAudio);
    shader.Use();
    shader.SetVector3f("color", glm::vec3(1.0f, 0.0f, 0.0f));
    damageTime = glfwGetTime();

    if (hp == 0)
    {
        deathState = 1; //enter dying state
        game.audioEngine->play(deathAudio);
    }

    return hp;
}

void MonkeyBoss::updateDeathTexture(float dt)
{
    deathUpdateTimer += dt;
    if (!(deathUpdateTimer >= deathUpdateDelay))
        return;
    deathUpdateTimer = 0.0f;
    if (deathTextureIndex < deathTextures.size() - 1) //if death animation not complete, update the texture
        currentTexture = deathTextures[++deathTextureIndex];
    else
    { //otherwise, set death state to 2 to indicate that the boss should be deleted
        deathState = 2;
    }
}

std::vector<GameObject> &MonkeyBoss::getBulletInfo()
{
    return bullets;
}

void MonkeyBoss::updateTransMatrices()
{ //update the transformation matrices
    glm::vec2 adjustedPosition = glm::vec2(Position.x - game.Width / 2, Position.y - game.Height / 2);
    adjustedPosition.x /= game.Width;
    adjustedPosition.y /= -game.Height;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(adjustedPosition, -1.5f));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    shader.SetMatrix4("model", model);
    shader.SetMatrix4("projection", projection);
}