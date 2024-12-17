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

    // Schaltet Musik an/aus
    void toggleMusic();

    void setMusicOn(bool v);

    // Stellt die Lautstärke ein
    void adjustVolume(int delta);

    // Gibt die aktuelle Lautstärke zurück
    int getVolume() const;

    // Gibt den Status der Musik zurück (ob Musik an oder aus ist)
    bool isMusicOn() const;

private:
    std::unordered_map<std::string, Mix_Chunk*> sounds; // Für Soundeffekte
    Mix_Music* music = nullptr; // Für Musik

    bool musicOn; // Musik ein/aus
    int volume;   // Lautstärke (0-100)
};

#endif // SOUNDMANAGER_H
