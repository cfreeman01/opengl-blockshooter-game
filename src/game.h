#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "soloud.h"
#include "soloud_wav.h"

//FORWARD DECLARATIONS
class GameObject;
class SpriteRenderer;
class Player;
class level;
class HUD;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_OVER,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    GameState               State;
    SpriteRenderer*         Renderer;
    Player*           currentPlayer;
    GameObject*             Cursor;
    level*                  currentLevel;
    HUD*                    hud;
    //input
    bool                    Keys[1024];
    bool                    mouse1, mouse2;
    float                   mouseX, mouseY;
    //dimensions
    int                     Width, Height;
    int                     playAreaHeight;
    //timing
    float                   elapsedTime = 0.0f;
    float                   lastDifficultyUpdate = 0.0f; //time at which difficulty was last increased

    //background
    std::vector<GameObject> background;
    void drawBackground(float dt);

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();

    // initialize game state (load all shaders/textures/levels)
    void Init();
    
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render(float dt);

    //audio
    SoLoud::Soloud* audioEngine = nullptr; //initialize SoLoud
    SoLoud::Wav restartAudio;

    //collisions
    void doCollisions();

    //change state
    void gameOver();
    void restart();
};