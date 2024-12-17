#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ConfigManager {
public:
    ConfigManager(const std::string& filename);
    ~ConfigManager();

    bool load();
    bool save();

    int getHighScore() const;
    void setHighScore(int score);

    bool isMusicOn() const;
    void setMusic(bool musicOn);

    int getVolume() const;
    void setVolume(int volume);

private:
    std::string filename;
    int highScore;
    bool musicOn;
    int volume;

    std::string getLineValue(const std::string& line, const std::string& key);
    void setKeyValue(std::ofstream& file, const std::string& key, const std::string& value);
};

#endif
