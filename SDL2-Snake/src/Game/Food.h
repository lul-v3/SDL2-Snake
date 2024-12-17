#ifndef FOOD_H
#define FOOD_H

#include <SDL2/SDL.h>
#include <vector>

class Food {
public:
    Food(int gridSize);

    void spawn(int width, int height, const std::vector<SDL_Point>& snakeBody);
    void render(SDL_Renderer* renderer);

    SDL_Point getPosition() const;

private:
    SDL_Point position;
    int gridSize;
};

#endif
