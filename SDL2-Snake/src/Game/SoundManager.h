#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    // Lädt einen Sound
    bool loadSound(const std::string& name, const std::string& path);

    // Spielt einen Sound ab
    void playSound(const std::string& name);

    // Lädt Musik
    bool loadMusic(const std::string& path);

    // Spielt Musik ab
    void playMusic(int loop = -1);

    // Stoppt Musik
    void stopMusic();

private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // Für Soundeffekte
    Mix_Music* music = nullptr; // Für Musik
};

#endif // SOUNDMANAGER_H
