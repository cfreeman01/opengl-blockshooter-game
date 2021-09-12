#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "soloud.h"
#include "soloud_wav.h"

//FORWARD DECLARATIONS
class gameObject;
class SpriteRenderer;
class playerObject;
class level;

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
    playerObject*           Player;
    gameObject*             Cursor;
    level*                  currentLevel;
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
    std::vector<gameObject> background;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render(float dt);
    //HUD and background
    void initHUD();
    void initNumbers();
    void drawHUD();
    void drawBackground(float dt);
    void renderTimer();

    //audio
    SoLoud::Soloud* audioEngine = nullptr; //initialize SoLoud
    SoLoud::Wav restartAudio;

    //collisions
    void doCollisions();

    //change state
    void gameOver();
    void restart();
};