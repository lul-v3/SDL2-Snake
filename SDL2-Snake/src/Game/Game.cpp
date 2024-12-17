#include "Game.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

// Hilfsfunktion für Text-Rendering
// Hilfsfunktion für Text-Rendering mit Zentrierung
void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    static TTF_Font* font = nullptr;
    if (!font) {
        font = TTF_OpenFont("assets/fonts/Daydream.ttf", 28); // Stelle sicher, dass die Font-Datei vorhanden ist
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }
    }

    // Berechne die Textbreite und -höhe
    int textWidth, textHeight;
    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

    // Berechne die Position, um den Text zu zentrieren
    int xPos = (x - textWidth / 2);
    int yPos = (y - textHeight / 2);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = { xPos, yPos, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


Game::Game(const std::string& title, int width, int height, int gridSize)
    : window(title, width, height), snake(gridSize), food(gridSize),
    running(true), gridSize(gridSize), state(GameState::MENU), menuSelection(0), screenWidth(width), screenHeight(height) {
    cols = width / gridSize;
    rows = height / gridSize;

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        running = false;
    }
}

Game::~Game() {
    TTF_Quit();
}

void Game::run() {
    // Laden der Sounds und Musik
    soundManager.loadSound("menuClick", "assets/sounds/menu_click.wav");
    soundManager.loadSound("menuSelect", "assets/sounds/menu_select.wav");
    soundManager.loadSound("eat", "assets/sounds/eat_sound.wav");
    soundManager.loadSound("gameOver", "assets/sounds/game_over.wav");
    soundManager.loadSound("eat", "assets/sounds/eat_sound.wav");
    soundManager.loadMusic("assets/music/PixelDreams.mp3");

    soundManager.playMusic(); // Hintergrundmusik starten

    while (running) {
        processInput();

        // Spielmodus wird nur aufgerufen, wenn state == PLAYING
        if (state == GameState::PLAYING) {
            update();
            render();
            SDL_Delay(100);
        }
        else if (state == GameState::MENU) {
            renderMenu();
        }
    }
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (state == GameState::MENU) {
            handleMenuInput(event);
        }
        else if (state == GameState::PLAYING) {
            snake.handleInput(event);
        }
    }
}

void Game::update() {
    snake.move();

    // Debugging-Ausgabe für die Kollision
    std::cout << "Snake head position: (" << snake.getBody()[0].x << ", " << snake.getBody()[0].y << ")\n";

    // Prüfe auf Kollision
    if (snake.checkCollision(cols, rows)) {
        std::cout << "Game Over: Collision detected!" << std::endl;
        soundManager.playSound("gameOver");
        state = GameState::MENU; // Zurück ins Menü
    }

    // Überprüfe, ob die Schlange das Essen frisst
    if (snake.collidesWithPosition(food.getPosition())) {
        soundManager.playSound("eat");
        snake.grow(); // Schlange wächst
        food.spawn(cols, rows, snake.getBody()); // Neues Essen spawnen
    }
}


void Game::render() {
    window.clear();
    snake.render(window.getRenderer());
    food.render(window.getRenderer());
    window.present();
}

void Game::renderMenu() {
    SDL_Renderer* renderer = window.getRenderer();
    window.clear();

    // Menü Hintergrund
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, nullptr);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color highlight = { 255, 0, 0, 255 }; // Auswahlfarbe für Schaltflächen

    // Zum Beispiel im Menü:
    renderText(renderer, "SNAKE GAME",  screenWidth / 2, screenHeight / 4, white);  // Zentriert den Text


    // Play-Button
    if (menuSelection == 0) {
        renderText(renderer, "Play", screenWidth / 2, screenHeight / 2, highlight); // Hervorgehobene Farbe
    }
    else {
        renderText(renderer, "Play", screenWidth / 2, screenHeight / 2, white);
    }

    // Exit-Button
    if (menuSelection == 1) {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.7, highlight); // Hervorgehobene Farbe
    }
    else {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.7, white);
    }

    window.present();
}

void Game::handleMenuInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 1) % 2; // Wechseln zwischen Play und Exit
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 1) % 2; // Wechseln zwischen Play und Exit
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            soundManager.playSound("menuClick");
            if (menuSelection == 0) {
                // Wechsel in den Spielmodus
                state = GameState::PLAYING;
                food.spawn(cols, rows, snake.getBody()); // Positioniere das Essen
                snake.reset(); // Snake zurücksetzen, um das Spiel neu zu starten
            }
            else if (menuSelection == 1) {
                running = false; // Spiel beenden
            }
        }
    }
}
