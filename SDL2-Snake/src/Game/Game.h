#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Snake.h"
#include "Food.h"
#include "SoundManager.h" // Include the SoundManager

enum class GameState {
    MENU,
    PLAYING,
    EXIT
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
    void handleMenuInput(SDL_Event& event);

    SoundManager soundManager;  // SoundManager Instanz
    Window window;
    Snake snake;
    Food food;

    bool running;
    int gridSize;
    int cols, rows;

    GameState state; // Game State (Menu, Playing, Exit)
    int menuSelection;

    int screenWidth, screenHeight;
};

#endif
