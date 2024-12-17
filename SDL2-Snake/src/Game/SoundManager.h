#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    // L�dt einen Sound
    bool loadSound(const std::string& name, const std::string& path);

    // Spielt einen Sound ab
    void playSound(const std::string& name);

    // L�dt Musik
    bool loadMusic(const std::string& path);

    // Spielt Musik ab
    void playMusic(int loop = -1);

    // Stoppt Musik
    void stopMusic();

private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // F�r Soundeffekte
    Mix_Music* music = nullptr; // F�r Musik
};

#endif // SOUNDMANAGER_H
