#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
    // SDL_Mixer initialisieren
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "SDL_Mixer konnte nicht initialisiert werden! Fehler: " << Mix_GetError() << std::endl;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "SDL_Mixer konnte nicht ge�ffnet werden! Fehler: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    // Alle geladenen Sounds freigeben
    for (auto& pair : sounds) {
        Mix_FreeChunk(pair.second);
    }

    if (music) {
        Mix_FreeMusic(music);
    }

    Mix_CloseAudio();
    Mix_Quit();
}

bool SoundManager::loadSound(const std::string& name, const std::string& path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (!sound) {
        std::cerr << "Fehler beim Laden des Sounds: " << path << " - " << Mix_GetError() << std::endl;
        return false;
    }
    sounds[name] = sound;
    return true;
}

void SoundManager::playSound(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[name], 0); // -1 f�r den n�chsten verf�gbaren Kanal
    }
}

bool SoundManager::loadMusic(const std::string& path) {
    music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Fehler beim Laden der Musik: " << path << " - " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::playMusic(int loop) {
    if (music) {
        Mix_PlayMusic(music, loop); // -1 f�r unendliche Schleife
    }
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}
