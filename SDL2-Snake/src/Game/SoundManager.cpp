#include "SoundManager.h"
#include <iostream>
#include <algorithm>

SoundManager::SoundManager() : musicOn(true), volume(50) {
    // SDL_Mixer initialisieren
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "SDL_Mixer konnte nicht initialisiert werden! Fehler: " << Mix_GetError() << std::endl;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "SDL_Mixer konnte nicht geöffnet werden! Fehler: " << Mix_GetError() << std::endl;
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
        Mix_PlayChannel(-1, sounds[name], 0); // -1 für den nächsten verfügbaren Kanal
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
        Mix_PlayMusic(music, loop); // -1 für unendliche Schleife
    }
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}

void SoundManager::toggleMusic() {
    if (musicOn) {
        stopMusic(); // Stoppt die Musik
    }
    else {
        playMusic(); // Spielt die Musik
    }
    musicOn = !musicOn; // Wechsel den Status der Musik
}

void SoundManager::setMusicOn(bool v)
{
    musicOn = v;
}

void SoundManager::adjustVolume(int volume) {
    int sdlVolume = static_cast<int>(volume * 1.28);  // 100% entspricht 128 in SDL (0 - 128)

    // Setze Musiklautstärke
    Mix_VolumeMusic(sdlVolume);

    // Setze Lautstärke für alle Soundeffekte
    for (auto& sound : sounds) {
        Mix_VolumeChunk(sound.second, sdlVolume);
    }
}

int SoundManager::getVolume() const {
    return volume;
}

bool SoundManager::isMusicOn() const {
    return musicOn;
}
