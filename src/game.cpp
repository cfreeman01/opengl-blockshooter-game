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
#include "playerObject.h"
#include "character.h"
#include "blockObject.h"
#include "monkeyBoss.h"
#include "level.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy3_mask.h"

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
    blockObject::loadTextures();
    enemy1::loadTextures();
    enemy2::loadTextures();
    enemy3::loadTextures();
    enemy3_mask::loadTextures();
    monkeyBoss::loadTextures();

    //load audio files
    blockObject::loadAudio();
    enemy1::loadAudio();
    enemy2::loadAudio();
    enemy3::loadAudio();
    enemy3_mask::loadAudio();
    restartAudio.load("audio/restart.wav");

    //initialize player
    glm::vec2 playerSize = glm::vec2(Width / 60.0f);
    glm::vec2 playerVelocity = glm::vec2(Width / 4.0f);
    Player = new playerObject(glm::vec2(playAreaHeight / 2), playerSize, ResourceManager::GetTexture("player"), glm::vec3(1.0f, 1.0f, 1.0f),
                              playerVelocity, *Renderer, *this);

    //initialize cursor
    glm::vec2 cursorSize = glm::vec2(Width / 60.0f);
    Cursor = new gameObject(glm::vec2(Width / 2), glm::vec2(cursorSize), ResourceManager::GetTexture("cursor"), glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::vec2(0.0f, 0.0f));

    //initialize level
    currentLevel = new level(*Renderer, *this);
    currentLevel->addRow();

    //initialize background
    background.push_back(gameObject(glm::vec2(0.0f, 0.0f), glm::vec2(Width, playAreaHeight), ResourceManager::GetTexture("background"),
                                    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(Width / 18.0f, 0.0f)));

    //initialize HUD
    initHUD();

    //initialize audio
    audioEngine = new SoLoud::Soloud();
    audioEngine->init();
}

void Game::Update(float dt)
{
    elapsedTime += dt;
    Player->moveBullets(dt);

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
        currentLevel->updateEnemies(dt, Player->Position);
        doCollisions();
    }
}

void Game::ProcessInput(float dt)
{
    //move player
    Player->move(dt, Cursor->Position);
    //move cursor
    Cursor->Position = glm::vec2(mouseX, mouseY);

    //check for mouse clicks
    if (mouse1)
    {
        Player->fire(Cursor->Position, dt);
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
        drawHUD();
        //render player
        Player->Draw();
        //render bullets
        Player->renderBullets();
        //render cursor
        Cursor->Draw(*Renderer);
    }
    else if (State == GAME_OVER)
    {
        //render player
        Player->Draw();
        //render bullets
        Player->renderBullets();
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
    std::vector<character::Bullet> bulletInfo = Player->getBulletInfo();
    //check bullet collisions
    std::vector<bool> destroyedBullets = currentLevel->checkBulletsCollisions(bulletInfo);
    //destroy bullets
    Player->destroyBullets(destroyedBullets);
    //detect collision with level
    currentLevel->checkPlayerCollisions(Player);
}

void Game::initHUD()
{
    ResourceManager::LoadTexture("textures/hud.png", true, "HUD_background");
    ResourceManager::LoadTexture("textures/hp_bar3.png", true, "HP3");
    ResourceManager::LoadTexture("textures/hp_bar2.png", true, "HP2");
    ResourceManager::LoadTexture("textures/hp_bar1.png", true, "HP1");
    initNumbers();
}

void Game::drawHUD()
{
    //draw the hud background
    Renderer->DrawSprite(ResourceManager::GetTexture("HUD_background"), glm::vec2(0.0f, playAreaHeight), glm::vec2(Width, Height - playAreaHeight));

    //draw HP bar
    std::string hpTexture = "HP" + to_string(Player->getHP());
    Renderer->DrawSprite(ResourceManager::GetTexture(hpTexture.c_str()), glm::vec2(Width / 10, playAreaHeight + 60),
                         glm::vec2(Width / 9.0f, Width / 24.0f));

    //draw timer
    renderTimer();
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
    gameObject rightBackground = background[background.size() - 1];
    if (rightBackground.Position.x + rightBackground.Size.x < Width)
    {
        background.push_back(gameObject(glm::vec2(rightBackground.Position.x + rightBackground.Size.x, 0.0f),
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

void Game::initNumbers()
{
    ResourceManager::LoadTexture("textures/number0.png", true, "number0");
    ResourceManager::LoadTexture("textures/number1.png", true, "number1");
    ResourceManager::LoadTexture("textures/number2.png", true, "number2");
    ResourceManager::LoadTexture("textures/number3.png", true, "number3");
    ResourceManager::LoadTexture("textures/number4.png", true, "number4");
    ResourceManager::LoadTexture("textures/number5.png", true, "number5");
    ResourceManager::LoadTexture("textures/number6.png", true, "number6");
    ResourceManager::LoadTexture("textures/number7.png", true, "number7");
    ResourceManager::LoadTexture("textures/number8.png", true, "number8");
    ResourceManager::LoadTexture("textures/number9.png", true, "number9");
    ResourceManager::LoadTexture("textures/number_colon.png", true, "colon");
}
void Game::renderTimer()
{
    //sizes of timer componenets

    glm::vec2 colonSize = glm::vec2(Width/32.0f, Width/18.0f);
    glm::vec2 colonPosition = glm::vec2(Width / 2 - colonSize.x / 2, Height - (Height - playAreaHeight) / 2 - colonSize.y / 2);
    glm::vec2 numberSize = glm::vec2(Width/18.0f);
    //calculate the digits for the timer
    int minutesTens = ((int)elapsedTime / 60) / 10;
    int minutesOnes = ((int)elapsedTime / 60) % 10;
    int secondsTens = ((int)elapsedTime % 60) / 10;
    int secondsOnes = ((int)elapsedTime % 60) % 10;
    //draw colon
    Renderer->DrawSprite(ResourceManager::GetTexture("colon"), colonPosition, colonSize);
    //draw digits
    Renderer->DrawSprite(ResourceManager::GetTexture("number" + to_string(minutesTens)), colonPosition - glm::vec2(2 * numberSize.x, 0), numberSize);
    Renderer->DrawSprite(ResourceManager::GetTexture("number" + to_string(minutesOnes)), colonPosition - glm::vec2(numberSize.x, 0), numberSize);
    Renderer->DrawSprite(ResourceManager::GetTexture("number" + to_string(secondsTens)), colonPosition + glm::vec2(colonSize.x, 0), numberSize);
    Renderer->DrawSprite(ResourceManager::GetTexture("number" + to_string(secondsOnes)), colonPosition + glm::vec2(colonSize.x + numberSize.x, 0), numberSize);
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
    Player->reset();
}