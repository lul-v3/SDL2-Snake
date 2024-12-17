#include "Window.h"
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    : window(nullptr), renderer(nullptr), closed(false) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        closed = true;
        return;
    }

    // Create window
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        closed = true;
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        closed = true;
    }
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window* Window::getSDLWindow() const {
    return window;
}

SDL_Renderer* Window::getRenderer() const {
    return renderer;
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Window::present() {
    SDL_RenderPresent(renderer);
}

bool Window::isClosed() const {
    return closed;
}
