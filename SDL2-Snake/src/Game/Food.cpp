#include "Food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int gridSize) : gridSize(gridSize) {
    std::srand(std::time(nullptr));
    position = { 0, 0 };
}

void Food::spawn(int cols, int rows, const std::vector<SDL_Point>& snakeBody) {
    bool validPosition = false;
    while (!validPosition) {
        validPosition = true;
        position.x = rand() % cols;
        position.y = rand() % rows;

        // Überprüfen, ob das Food auf der Schlange ist
        for (const SDL_Point& bodyPart : snakeBody) {
            if (bodyPart.x == position.x && bodyPart.y == position.y) {
                validPosition = false;  // Position ist von der Schlange besetzt
                break;
            }
        }
    }
}

void Food::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { position.x * gridSize, position.y * gridSize, gridSize, gridSize };
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Point Food::getPosition() const {
    return position;
}
