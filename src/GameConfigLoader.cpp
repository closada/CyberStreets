#include "GameConfigLoader.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

GameConfig GameConfigLoader::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir config general: " + path);

    json j;
    file >> j;

    GameConfig config;

    config.name = j["name"];
    config.typography = j["typografy"];
    config.score = j["score"];
    config.lastLevelCompleted = j["last_level_completed"];
    config.music = j["music"];

    for (const auto& lvl : j["levels"])
    {
        LevelEntry entry;
        entry.id = lvl["id"];
        entry.file = lvl["file"];

        config.levels.push_back(entry);
    }

    // los distintos items de sonido
    for (auto it = j["sounds"].begin(); it != j["sounds"].end(); ++it)
    {
        config.sounds[it.key()] = it.value();
    }


    return config;
}

