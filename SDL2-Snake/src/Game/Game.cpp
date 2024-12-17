#include "Game.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color, int size) {
    static TTF_Font* font = nullptr;
    if (!font) {
        font = TTF_OpenFont("assets/fonts/Daydream.ttf", size);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }
    }

    int textWidth, textHeight;
    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
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
    running(true), gridSize(gridSize), state(GameState::MENU), menuSelection(0), screenWidth(width), screenHeight(height),
    currentScore(0), highScore(0), musicOn(true), volume(50), audioMenuSelection(0), lastScore(0), config("settings.ini") {

    cols = width / gridSize;
    rows = height / gridSize;

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        running = false;
    }

    soundManager.loadSound("menuClick", "assets/sounds/menu_click.wav");
    soundManager.loadSound("menuSelect", "assets/sounds/menu_select.wav");
    soundManager.loadSound("eat", "assets/sounds/eat_sound.wav");
    soundManager.loadSound("gameOver", "assets/sounds/game_over.wav");
    soundManager.loadMusic("assets/music/PixelDreams.mp3");

    // Laden der Einstellungen und des Highscores
    if (!config.load()) {
        std::cerr << "Error loading settings!" << std::endl;
    }
    else {
        std::cout << "[LOADED CONFIG]" << std::endl;
        // load settings
        highScore = config.getHighScore();
        std::cout << "LOADED HIGHSCORE: " << highScore << std::endl;
        musicOn = config.isMusicOn();
        std::cout << "LOADED MUSIC STATE: " << musicOn << std::endl;
        volume = config.getVolume();
        std::cout << "LOADED VOLUME: " << volume << std::endl;

        // set settings
        soundManager.setMusicOn(musicOn);
        soundManager.adjustVolume(volume);

    }

    if (musicOn) { soundManager.playMusic(); }
}

Game::~Game() {
    TTF_Quit();
}

void Game::run() {
    while (running) {
        processInput();

        if (state == GameState::PLAYING) {
            update();
            render();
            SDL_Delay(100);
        }
        else if (state == GameState::MENU) {
            renderMenu();
        }
        else if (state == GameState::GAME_OVER) {
            renderGameOver();
        }
        else if (state == GameState::PAUSED) {
            renderPauseMenu();
        }
        else if (state == GameState::AUDIO_SETTINGS) {
            renderAudioSettingsMenu();
        }
    }

    // Speichern der Einstellungen und des Highscores beim Beenden
    config.setHighScore(highScore);
    config.setMusic(musicOn);
    config.setVolume(volume);
    if (!config.save()) {
        std::cerr << "Error saving settings!" << std::endl;
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
        else if (state == GameState::GAME_OVER) {
            handleMenuInput(event);
        }
        else if (state == GameState::PAUSED) {
            handlePauseMenuInput(event);
        }
        else if (state == GameState::AUDIO_SETTINGS) {
            handleAudioSettingsInput(event);
        }
        else if (state == GameState::PLAYING) {
            snake.handleInput(event);

            // ESC-Taste für Pause hinzufügen
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                state = GameState::PAUSED;
            }
        }
    }
}


void Game::update() {
    snake.move();

    if (snake.checkCollision(cols, rows)) {
        std::cout << "Game Over: Collision detected!" << std::endl;
        soundManager.playSound("gameOver");

        lastScore = currentScore;
        if (lastScore > highScore) {
            highScore = lastScore;
        }
        state = GameState::GAME_OVER;
        resetGame();
    }

    if (snake.collidesWithPosition(food.getPosition())) {
        soundManager.playSound("eat");
        currentScore += 10;
        snake.grow();
        food.spawn(cols, rows, snake.getBody());
    }
}

void Game::render() {
    window.clear();
    snake.render(window.getRenderer());
    food.render(window.getRenderer());

    SDL_Color white = { 255, 255, 255, 255 };
    renderText(window.getRenderer(), "Score: " + std::to_string(currentScore), screenWidth / 2, 30, white, 28);

    window.present();
}

void Game::renderMenu() {
    SDL_Renderer* renderer = window.getRenderer();
    window.clear();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, nullptr);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color highlight = { 255, 0, 0, 255 };

    renderText(renderer, "SNAKE GAME", screenWidth / 2, screenHeight / 4, white, 20);

    if (menuSelection == 0) {
        renderText(renderer, "Play", screenWidth / 2, screenHeight / 2, highlight, 20);
    }
    else {
        renderText(renderer, "Play", screenWidth / 2, screenHeight / 2, white, 20);
    }

    if (menuSelection == 1) {
        renderText(renderer, "Audio Settings", screenWidth / 2, screenHeight / 1.7, highlight, 20);
    }
    else {
        renderText(renderer, "Audio Settings", screenWidth / 2, screenHeight / 1.7, white, 20);
    }

    if (menuSelection == 2) {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.3, highlight, 20);
    }
    else {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.3, white, 20);
    }

    window.present();
}


void Game::renderGameOver() {
    SDL_Renderer* renderer = window.getRenderer();
    window.clear();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, nullptr);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color highlight = { 255, 0, 0, 255 }; // Auswahlfarbe für hervorgehobene Menüpunkte

    // "Game Over" Text (groß)
    renderText(renderer, "GAME OVER", screenWidth / 2, screenHeight / 4, white, 28);

    // Highscore Text (klein, über dem aktuellen Score)
    renderText(renderer, "Highscore: " + std::to_string(highScore), screenWidth / 2, screenHeight / 3, white, 15);

    // Aktuellen Score in größerer Schrift unter dem Highscore
    renderText(renderer, "Score: " + std::to_string(lastScore), screenWidth / 2, screenHeight / 2, white, 18);

    // Auswahlmöglichkeiten (kleinerer Text für Buttons)
    if (menuSelection == 0) {
        renderText(renderer, "Try Again", screenWidth / 2, screenHeight / 1.7, highlight, 20);
    }
    else {
        renderText(renderer, "Try Again", screenWidth / 2, screenHeight / 1.7, white, 20);
    }

    if (menuSelection == 1) {
        renderText(renderer, "Main Menu", screenWidth / 2, screenHeight / 1.5, highlight, 20);
    }
    else {
        renderText(renderer, "Main Menu", screenWidth / 2, screenHeight / 1.5, white, 20);
    }

    if (menuSelection == 2) {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.1, highlight, 20);
    }
    else {
        renderText(renderer, "Exit", screenWidth / 2, screenHeight / 1.1, white, 20);
    }

    window.present();
}




void Game::renderPauseMenu() {
    SDL_Renderer* renderer = window.getRenderer();
    window.clear();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, nullptr);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color highlight = { 255, 0, 0, 255 };

    renderText(renderer, "PAUSED", screenWidth / 2, screenHeight / 4, white, 28);

    if (menuSelection == 0) {
        renderText(renderer, "Resume", screenWidth / 2, screenHeight / 2, highlight, 20);
    }
    else {
        renderText(renderer, "Resume", screenWidth / 2, screenHeight / 2, white, 20);
    }

    if (menuSelection == 1) {
        renderText(renderer, "Main Menu", screenWidth / 2, screenHeight / 1.7, highlight, 20);
    }
    else {
        renderText(renderer, "Main Menu", screenWidth / 2, screenHeight / 1.7, white, 20);
    }

    if (menuSelection == 2) {
        renderText(renderer, "Exit Game", screenWidth / 2, screenHeight / 1.3, highlight, 20);
    }
    else {
        renderText(renderer, "Exit Game", screenWidth / 2, screenHeight / 1.3, white, 20);
    }

    window.present();
}

void Game::renderAudioSettingsMenu() {
    SDL_Renderer* renderer = window.getRenderer();
    window.clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color highlight = { 255, 0, 0, 255 }; // Auswahlfarbe für Schaltflächen

    // Musik ON/OFF Text
    std::string musicText = musicOn ? "Music: ON" : "Music: OFF";

    // Lautstärke Text
    std::string volumeText = "Volume: " + std::to_string(volume) + "%";

    // Musik ein/aus schalten: Highlight für Musik
    if (audioMenuSelection == 0) {
        renderText(renderer, musicText, screenWidth / 2, screenHeight / 3, highlight, 20); // Musik wird hervorgehoben
    }
    else {
        renderText(renderer, musicText, screenWidth / 2, screenHeight / 3, white, 20);
    }

    // Lautstärke ändern: Highlight für Lautstärke
    if (audioMenuSelection == 1) {
        renderText(renderer, volumeText, screenWidth / 2, screenHeight / 2, highlight, 20); // Lautstärke wird hervorgehoben
    }
    else {
        renderText(renderer, volumeText, screenWidth / 2, screenHeight / 2, white, 20);
    }

    // Zurück Button
    if (audioMenuSelection == 2) {
        renderText(renderer, "Back", screenWidth / 2, screenHeight / 1.7, highlight, 20); // „Back“ wird hervorgehoben
    }
    else {
        renderText(renderer, "Back", screenWidth / 2, screenHeight / 1.7, white, 20);
    }

    window.present();
}


void Game::handleMenuInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 1) % 3;
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 2) % 3;  // sorgt dafür, dass die Auswahl korrekt hochgeht
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            soundManager.playSound("menuClick");
            if (menuSelection == 0) {
                state = GameState::PLAYING;
            }
            else if (menuSelection == 1) {
                state = GameState::AUDIO_SETTINGS;
            }
            else if (menuSelection == 2) {
                running = false;
            }
        }
    }
}



void Game::handleAudioSettingsInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            soundManager.playSound("menuSelect");
            audioMenuSelection = (audioMenuSelection + 1) % 3; // Navigiere nach unten, 3 Optionen
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            soundManager.playSound("menuSelect");
            audioMenuSelection = (audioMenuSelection + 2) % 3; // Navigiere nach oben
        }
        else if (event.key.keysym.sym == SDLK_LEFT && audioMenuSelection == 1) {
            // Lautstärke verringern
            volume = (volume > 0) ? volume - 10 : 0;
            soundManager.adjustVolume(volume);
        }
        else if (event.key.keysym.sym == SDLK_RIGHT && audioMenuSelection == 1) {
            // Lautstärke erhöhen
            volume = (volume < 100) ? volume + 10 : 100;
            soundManager.adjustVolume(volume);
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            soundManager.playSound("menuClick");
            if (audioMenuSelection == 0) {
                // Musik ein/aus schalten
                musicOn = !musicOn;
                if (musicOn) {
                    soundManager.playMusic();
                }
                else {
                    soundManager.stopMusic();
                }
            }
            else if (audioMenuSelection == 1) {
                // Lautstärke ändern (bereits durch Pfeiltasten behandelt)
            }
            else if (audioMenuSelection == 2) {
                // Zurück
                state = GameState::MENU;
            }
        }
    }
}


void Game::handlePauseMenuInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 1) % 3;
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 2) % 3;
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            soundManager.playSound("menuClick");
            if (menuSelection == 0) {
                state = GameState::PLAYING;
            }
            else if (menuSelection == 1) {
                state = GameState::MENU;
            }
            else if (menuSelection == 2) {
                running = false;
            }
        }
    }
}

void Game::handleGameOverMenuInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 1) % 3;  // Für 3 Optionen
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            soundManager.playSound("menuSelect");
            menuSelection = (menuSelection + 2) % 3;  // Nach oben navigieren
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {
            soundManager.playSound("menuClick");
            if (menuSelection == 0) {
                // Spiel zurücksetzen, Score zurücksetzen
                resetGame();
                currentScore = 0;  // Den Score zurücksetzen
                state = GameState::PLAYING;  // Zurück ins Spiel
            }
            else if (menuSelection == 1) {
                state = GameState::MENU;  // Ins Hauptmenü zurück
                currentScore = 0;  // Score zurücksetzen
            }
            else if (menuSelection == 2) {
                running = false;  // Spiel beenden
            }
        }
    }
}


void Game::resetGame()
{
    // Snake zurücksetzen (z. B. Position und Größe der Schlange zurücksetzen)
    snake.reset(cols, rows);
    currentScore = 0;
    // Food an einer neuen Position spawnen (unter der Annahme, dass die spawn-Methode auch die Größe der Schlange berücksichtigt)
    food.spawn(cols, rows, snake.getBody()); // Wenn die spawn-Methode den Snake-Körper berücksichtigt, wird die Nahrung nicht an der Schlange erscheinen
}
