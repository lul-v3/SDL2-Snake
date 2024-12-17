#include "ConfigManager.h"
#include <filesystem>

ConfigManager::ConfigManager(const std::string& filename)
    : filename(filename), highScore(0), musicOn(true), volume(50) {
}

ConfigManager::~ConfigManager() {}

// Load function (mit Dateiexistenzprüfung)
bool ConfigManager::load() {
    // Überprüfen, ob die Datei existiert
    if (!std::filesystem::exists(filename)) {
        std::cout << "Config file not found. Creating a new one with default settings." << std::endl;
        return save(); // Wenn die Datei nicht existiert, erstelle sie mit Standardwerten
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for reading." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("HighScore") != std::string::npos) {
            highScore = std::stoi(getLineValue(line, "HighScore"));
        }
        else if (line.find("Music") != std::string::npos) {
            musicOn = (getLineValue(line, "Music") == "ON");
        }
        else if (line.find("Volume") != std::string::npos) {
            volume = std::stoi(getLineValue(line, "Volume"));
        }
    }

    file.close();
    return true;
}

// Save function
bool ConfigManager::save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing." << std::endl;
        return false;
    }

    file << "[Settings]\n";
    setKeyValue(file, "HighScore", std::to_string(highScore));
    setKeyValue(file, "Music", musicOn ? "ON" : "OFF");
    setKeyValue(file, "Volume", std::to_string(volume));

    file.close();
    return true;
}

int ConfigManager::getHighScore() const {
    return highScore;
}

void ConfigManager::setHighScore(int score) {
    highScore = score;
}

bool ConfigManager::isMusicOn() const {
    return musicOn;
}

void ConfigManager::setMusic(bool musicOn) {
    this->musicOn = musicOn;
}

int ConfigManager::getVolume() const {
    return volume;
}

void ConfigManager::setVolume(int volume) {
    this->volume = volume;
}

std::string ConfigManager::getLineValue(const std::string& line, const std::string& key) {
    size_t pos = line.find(key);
    if (pos != std::string::npos) {
        return line.substr(pos + key.length() + 1); // Skip key and equals sign
    }
    return "";
}

void ConfigManager::setKeyValue(std::ofstream& file, const std::string& key, const std::string& value) {
    file << key << "=" << value << "\n";
}
