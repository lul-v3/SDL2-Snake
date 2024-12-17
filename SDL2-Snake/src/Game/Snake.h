#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <SDL2/SDL.h>
#include "Food.h"

enum class Direction {
    UP, DOWN, LEFT, RIGHT
};

class Snake {
public:
    Snake(int gridSize);
    void move();
    void render(SDL_Renderer* renderer);
    void handleInput(const SDL_Event& event);
    bool checkCollision(int cols, int rows);
    bool collidesWithPosition(const SDL_Point& foodPosition);
    void grow();
    void reset();

    const std::vector<SDL_Point>& getBody() const { return body; }

private:
    std::vector<SDL_Point> body;
    Direction direction;
    int growth;
    int gridSize;
};

#endif // SNAKE_H
