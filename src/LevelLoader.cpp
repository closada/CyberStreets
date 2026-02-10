#include "LevelLoader.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

LevelData LevelLoader::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir " + path);

    json j;
    file >> j;

    LevelData level;
    level.id          = j["id"];
    level.levelLength = j["levelLength"];
    level.goalX       = j["goalX"];
    level.groundY     = j["groundY"];

    for (const auto& w : j["waves"])
    {
        EnemyWave wave;
        wave.enemyId = w["type"];
        wave.x       = w["x"];
        wave.y       = w["y"];
        wave.delay   = w["delay"];
        wave.count   = w["count"];

        level.waves.push_back(wave);
    }

    return level;
}


