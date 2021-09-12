#pragma once

//FORWARD DECLARATIONS
class Game;

//renders the heads-up display at the bottom of the screen
class HUD
{
private:
    Game &game;
    void initNumbers();
    void renderTimer();

public:
    HUD(Game& game);
    void draw();
};