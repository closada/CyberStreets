#include "SaveManager.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

void SaveManager::saveProgress(const std::string& path, const GameConfig& config)
{
    std::ifstream inputFile(path);
    if (!inputFile.is_open())
        return;

    nlohmann::json j;
    inputFile >> j;
    inputFile.close();

    // Solo actualizamos el progreso
    j["last_level_completed"] = config.lastLevelCompleted;

    std::ofstream outputFile(path);
    outputFile << j.dump(4);
}

