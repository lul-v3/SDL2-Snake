#include "Snake.h"
#include <iostream>

Snake::Snake(int gridSize)
    : gridSize(gridSize), direction(Direction::RIGHT), growth(0) {
    body.push_back({ 5, 5 }); // Startposition der Schlange
}

void Snake::move() {
    // Schlange bewegen, abhängig von der Richtung
    SDL_Point newHead = body.front();

    switch (direction) {
    case Direction::UP:
        newHead.y -= 1;
        break;
    case Direction::DOWN:
        newHead.y += 1;
        break;
    case Direction::LEFT:
        newHead.x -= 1;
        break;
    case Direction::RIGHT:
        newHead.x += 1;
        break;
    }

    // Füge das neue Kopfteil an der Stelle der Schlange hinzu
    body.insert(body.begin(), newHead);

    // Wenn die Schlange gewachsen ist, füge kein neues Segment am Ende hinzu
    if (growth > 0) {
        growth--;
    }
    else {
        body.pop_back(); // Entferne das letzte Segment
    }
}

void Snake::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Setze die Schlange auf grün
    for (const SDL_Point& segment : body) {
        SDL_Rect rect = { segment.x * gridSize, segment.y * gridSize, gridSize, gridSize };
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Snake::handleInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            if (direction != Direction::DOWN) direction = Direction::UP;
            break;
        case SDLK_DOWN:
            if (direction != Direction::UP) direction = Direction::DOWN;
            break;
        case SDLK_LEFT:
            if (direction != Direction::RIGHT) direction = Direction::LEFT;
            break;
        case SDLK_RIGHT:
            if (direction != Direction::LEFT) direction = Direction::RIGHT;
            break;
        }
    }
}

bool Snake::checkCollision(int cols, int rows) {
    // Überprüfe Kollision mit sich selbst
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[0].x == body[i].x && body[0].y == body[i].y) {
            return true; // Kollision mit sich selbst
        }
    }

    // Überprüfe Kollision mit den Wänden
    if (body[0].x < 0 || body[0].x >= cols || body[0].y < 0 || body[0].y >= rows) {
        return true; // Kollision mit der Wand
    }

    return false; // Keine Kollision
}


bool Snake::collidesWithPosition(const SDL_Point& foodPosition) {
    return body[0].x == foodPosition.x && body[0].y == foodPosition.y;
}

void Snake::grow() {
    growth++; // Die Schlange wächst um ein Segment
}

void Snake::reset(int cols, int rows) {
    body.clear();
    body.push_back({ cols / 2, rows / 2 });  // Setze die Snake in die Mitte
    direction = Direction::LEFT;  // Setze die Bewegungsrichtung nach rechts
    growth = 1;
}



