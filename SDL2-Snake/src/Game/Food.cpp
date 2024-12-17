#include "Food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int gridSize) : gridSize(gridSize) {
    std::srand(std::time(nullptr));
    position = { 0, 0 };
}

void Food::spawn(int width, int height, const std::vector<SDL_Point>& snakeBody) {
    bool valid = false;
    while (!valid) {
        position.x = std::rand() % width;
        position.y = std::rand() % height;
        valid = true;
        for (const auto& segment : snakeBody) {
            if (segment.x == position.x && segment.y == position.y) {
                valid = false;
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
