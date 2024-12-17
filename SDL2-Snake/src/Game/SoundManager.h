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

    // Schaltet Musik an/aus
    void toggleMusic();

    void setMusicOn(bool v);

    // Stellt die Lautst�rke ein
    void adjustVolume(int delta);

    // Gibt die aktuelle Lautst�rke zur�ck
    int getVolume() const;

    // Gibt den Status der Musik zur�ck (ob Musik an oder aus ist)
    bool isMusicOn() const;

private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // F�r Soundeffekte
    Mix_Music* music = nullptr; // F�r Musik

    bool musicOn; // Musik ein/aus
    int volume;   // Lautst�rke (0-100)
};

#endif // SOUNDMANAGER_H
