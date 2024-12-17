#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    SDL_Window* getSDLWindow() const;
    SDL_Renderer* getRenderer() const;

    void clear();
    void present();

    bool isClosed() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool closed;
};

#endif
