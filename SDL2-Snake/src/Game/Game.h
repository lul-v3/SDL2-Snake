#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Snake.h"
#include "Food.h"
#include "SoundManager.h"
#include "ConfigManager.h"

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    EXIT,
    PAUSED,
    AUDIO_SETTINGS
};

class Game {
public:
    Game(const std::string& title, int width, int height, int gridSize);
    ~Game();

    void run();

private:
    void processInput();
    void update();
    void render();
    void renderMenu();
    void renderGameOver();
    void renderPauseMenu();
    void renderAudioSettingsMenu();
    void handleMenuInput(SDL_Event& event);  // Deklaration der Methode
    void handlePauseMenuInput(SDL_Event& event);
    void handleAudioSettingsInput(SDL_Event& event);
    void handleGameOverMenuInput(SDL_Event& event);
    void resetGame();

    void updateScore();

    SoundManager soundManager;
    Window window;
    Snake snake;
    Food food;
    ConfigManager config;

    bool running;
    int gridSize;
    int cols, rows;

    GameState state; // Game State (Menu, Playing, Exit, Game Over, Paused, Audio Settings)
    int menuSelection;
    int audioMenuSelection;

    int screenWidth, screenHeight;
    int currentScore, lastScore;
    int highScore;
    bool musicOn;
    int volume;
};

#endif
