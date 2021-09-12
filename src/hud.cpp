#include "hud.h"
#include "game.h"
#include "resource_manager.h"
#include "spriteRenderer.h"
#include "player.h"
#include <iostream>

HUD::HUD(Game& game): game(game)
{
    ResourceManager::LoadTexture("textures/hud.png", true, "HUD_background");
    ResourceManager::LoadTexture("textures/hp_bar3.png", true, "HP3");
    ResourceManager::LoadTexture("textures/hp_bar2.png", true, "HP2");
    ResourceManager::LoadTexture("textures/hp_bar1.png", true, "HP1");
    initNumbers();
}

void HUD::initNumbers()
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

void HUD::draw()
{
    //draw the hud background
    game.Renderer->DrawSprite(ResourceManager::GetTexture("HUD_background"),
                              glm::vec2(0.0f, game.playAreaHeight),
                              glm::vec2(game.Width, game.Height - game.playAreaHeight));

    //draw HP bar
    std::string hpTexture = "HP" + std::to_string(game.currentPlayer->getHP());
    game.Renderer->DrawSprite(ResourceManager::GetTexture(hpTexture.c_str()),
                              glm::vec2(game.Width / 10, game.playAreaHeight + 60),
                              glm::vec2(game.Width / 9.0f, game.Width / 24.0f));

    //draw timer
    renderTimer();
}

void HUD::renderTimer()
{
    //sizes of timer componenets
    glm::vec2 colonSize = glm::vec2(game.Width / 32.0f, game.Width / 18.0f);
    glm::vec2 colonPosition = glm::vec2(game.Width / 2 - colonSize.x / 2, game.Height - (game.Height - game.playAreaHeight) / 2 - colonSize.y / 2);
    glm::vec2 numberSize = glm::vec2(game.Width / 18.0f);

    //calculate the digits for the timer
    int minutesTens = ((int)game.elapsedTime / 60) / 10;
    int minutesOnes = ((int)game.elapsedTime / 60) % 10;
    int secondsTens = ((int)game.elapsedTime % 60) / 10;
    int secondsOnes = ((int)game.elapsedTime % 60) % 10;

    //draw colon
    game.Renderer->DrawSprite(ResourceManager::GetTexture("colon"), colonPosition, colonSize);

    //draw digits
    game.Renderer->DrawSprite(ResourceManager::GetTexture("number" + std::to_string(minutesTens)), colonPosition - glm::vec2(2 * numberSize.x, 0), numberSize);
    game.Renderer->DrawSprite(ResourceManager::GetTexture("number" + std::to_string(minutesOnes)), colonPosition - glm::vec2(numberSize.x, 0), numberSize);
    game.Renderer->DrawSprite(ResourceManager::GetTexture("number" + std::to_string(secondsTens)), colonPosition + glm::vec2(colonSize.x, 0), numberSize);
    game.Renderer->DrawSprite(ResourceManager::GetTexture("number" + std::to_string(secondsOnes)), colonPosition + glm::vec2(colonSize.x + numberSize.x, 0), numberSize);
}