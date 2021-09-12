#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <string>
#include "game.h"
#include "shader.h"
#include "resource_manager.h"
#include "spriteRenderer.h"
#include "gameObject.h"
#include "player.h"
#include "character.h"
#include "block.h"
#include "monkeyBoss.h"
#include "level.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy3_mask.h"
#include "hud.h"

using namespace std;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    mouseX = (float)this->Width / 2;
    mouseY = (float)this->Height / 2;
    mouse1 = mouse2 = false;

    playAreaHeight = Height - (Height / 5);
}

Game::~Game()
{
}

void Game::Init()
{
    //load textures
    ResourceManager::LoadTexture("textures/player.png", true, "player");
    ResourceManager::LoadTexture("textures/cursor.png", true, "cursor");
    ResourceManager::LoadTexture("textures/bullet_base.png", true, "bullet");
    ResourceManager::LoadTexture("textures/bullet_circle.png", true, "bullet_circle");
    ResourceManager::LoadTexture("textures/background.png", true, "background");
    ResourceManager::LoadTexture("textures/hud.png", true, "HUD_background");
    ResourceManager::LoadTexture("textures/gameover.png", true, "gameover");
    ResourceManager::LoadTexture("textures/gamewin.png", true, "gamewin");
    Block::loadTextures();
    Enemy1::loadTextures();
    Enemy2::loadTextures();
    Enemy3::loadTextures();
    Enemy3_Mask::loadTextures();
    MonkeyBoss::loadTextures();

    //load audio files
    Block::loadAudio();
    Enemy1::loadAudio();
    Enemy2::loadAudio();
    Enemy3::loadAudio();
    Enemy3_Mask::loadAudio();
    restartAudio.load("audio/restart.wav");

    //initialize player
    glm::vec2 playerSize = glm::vec2(Width / 60.0f);
    glm::vec2 playerVelocity = glm::vec2(Width / 4.0f);
    currentPlayer = new Player(glm::vec2(playAreaHeight / 2), playerSize, ResourceManager::GetTexture("player"), glm::vec3(1.0f, 1.0f, 1.0f),
                              playerVelocity, *Renderer, *this);

    //initialize cursor
    glm::vec2 cursorSize = glm::vec2(Width / 60.0f);
    Cursor = new GameObject(glm::vec2(Width / 2), glm::vec2(cursorSize), ResourceManager::GetTexture("cursor"), glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::vec2(0.0f, 0.0f));

    //initialize level
    currentLevel = new level(*Renderer, *this);
    currentLevel->addRow();

    //initialize background
    background.push_back(GameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, playAreaHeight), ResourceManager::GetTexture("background"),
                                    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(Width / 18.0f, 0.0f)));

    //initialize HUD
    hud = new HUD(*this);

    //initialize audio
    audioEngine = new SoLoud::Soloud();
    audioEngine->init();
}

void Game::Update(float dt)
{
    elapsedTime += dt;
    currentPlayer->moveBullets(dt);

    if (State == GAME_ACTIVE)
    {
        //possibly increase difficulty
        if (elapsedTime - lastDifficultyUpdate > 30)
        { //update difficulty every 30 seconds
            lastDifficultyUpdate = elapsedTime;
            currentLevel->increaseDifficulty();
        }

        //update level and detect collisions
        currentLevel->moveBlocks(dt);
        currentLevel->updateEnemies(dt, currentPlayer->Position);
        doCollisions();
    }
}

void Game::ProcessInput(float dt)
{
    //move player
    currentPlayer->move(dt, Cursor->Position);
    //move cursor
    Cursor->Position = glm::vec2(mouseX, mouseY);

    //check for mouse clicks
    if (mouse1)
    {
        currentPlayer->fire(Cursor->Position, dt);
        if (State == GAME_OVER)
            restart();
    }
}

void Game::Render(float dt)
{
    if (State == GAME_ACTIVE || State == GAME_WIN)
    {
        //render backgeound
        drawBackground(dt);
        //render level
        currentLevel->drawBlocks();
        //render enemies
        currentLevel->renderEnemies();
        //render HUD
        hud->draw();
        //render player
        currentPlayer->Draw();
        //render bullets
        currentPlayer->renderBullets();
        //render cursor
        Cursor->Draw(*Renderer);
    }
    else if (State == GAME_OVER)
    {
        //render player
        currentPlayer->Draw();
        //render bullets
        currentPlayer->renderBullets();
        //render cursor
        Cursor->Draw(*Renderer);
        //render game over message
        float gameOverSize = Width / 4.5f;
        Renderer->DrawSprite(ResourceManager::GetTexture("gameover"), glm::vec2(Width / 2 - gameOverSize / 2, Height / 2 - gameOverSize / 2),
                             glm::vec2(gameOverSize, gameOverSize));
    }

    if (State == GAME_WIN)
    { //render game win message
        float gameWinSize = Width / 4.5f;
        Renderer->DrawSprite(ResourceManager::GetTexture("gamewin"), glm::vec2(Width / 2 - gameWinSize / 2, Height / 2 - gameWinSize / 2),
                             glm::vec2(gameWinSize, gameWinSize));
    }
}

void Game::doCollisions()
{
    //get bullet info
    std::vector<Character::Bullet> bulletInfo = currentPlayer->getBulletInfo();
    //check bullet collisions
    std::vector<bool> destroyedBullets = currentLevel->checkBulletsCollisions(bulletInfo);
    //destroy bullets
    currentPlayer->destroyBullets(destroyedBullets);
    //detect collision with level
    currentLevel->checkPlayerCollisions(currentPlayer);
}

void Game::drawBackground(float dt)
{
    //move the background
    for (int i = 0; i < background.size(); i++)
    {
        background[i].Position.x -= background[i].Velocity.x * dt;
    }
    //add another copy of the background next to old one if needed
    //(to create seamless scrolling effect)
    GameObject rightBackground = background[background.size() - 1];
    if (rightBackground.Position.x + rightBackground.Size.x < Width)
    {
        background.push_back(GameObject(glm::vec2(rightBackground.Position.x + rightBackground.Size.x, 0.0f),
                                        glm::vec2(Width, playAreaHeight), ResourceManager::GetTexture("background"), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(Width / 18.0f, 0.0f)));
    }
    //delete background if needed
    if (background[0].Position.x < -background[0].Size.x)
    {
        background.erase(background.begin());
    }
    //draw the background
    for (int i = 0; i < background.size(); i++)
    {
        background[i].Draw(*Renderer);
    }
}

void Game::gameOver()
{ //when player dies, set the state delete the level
    State = GAME_OVER;
    delete currentLevel;
    currentLevel = nullptr;
}

void Game::restart()
{ //after a game over, restart the game
    audioEngine->play(restartAudio);
    State = GAME_ACTIVE;
    elapsedTime = 0;
    lastDifficultyUpdate = 0;
    currentLevel = new level(*Renderer, *this);
    currentLevel->addRow();
    currentPlayer->reset();
}