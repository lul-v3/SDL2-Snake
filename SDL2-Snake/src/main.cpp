#define SDL_MAIN_HANDLED
#include "Game/Game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int gridSize = 20;

    Game game("Snake Game", screenWidth, screenHeight, gridSize);
    game.run();

    return 0;
}
