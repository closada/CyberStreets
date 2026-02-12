#include "EnemyConfigLoader.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

EnemyConfigLoader::EnemyConfigLoader(
    ResourceManager<sf::Texture, std::string>& enemyTextures
)
: textures(enemyTextures)
{

    std::cout << "prueba constructor enemyConfigLoader." << std::endl;
}

void EnemyConfigLoader::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open EnemyTypes.json");

    json data;
    file >> data;

    for (auto& [enemyId, value] : data.items())
    {
        EnemyConfig config;

        config.maxHealth      = value["health"];
        config.damage         = value["damage"];
        config.attackCooldown = value["attackCooldown"];
        config.speed          = value["speed"];

        // ---------- RUN ----------
        {
            std::string path = value["sprite_run"]["link"];
            int frames       = value["sprite_run"]["cant_sprites"];

            std::string texId = "enemy." + enemyId + ".run";
            textures.load(texId, path);

            config.runTexId   = texId;
            config.runFrames = frames;
        }

        // ---------- ATTACK ----------
        {
            std::string path = value["sprite_attack"]["link"];
            int frames       = value["sprite_attack"]["cant_sprites"];

            std::string texId = "enemy." + enemyId + ".attack";
            textures.load(texId, path);

            config.attackTexId   = texId;
            config.attackFrames = frames;
        }

        // ---------- DEAD ----------
        {
            std::string path = value["sprite_dead"]["link"];
            int frames       = value["sprite_dead"]["cant_sprites"];

            std::string texId = "enemy." + enemyId + ".dead";
            textures.load(texId, path);

            config.deadTexId   = texId;
            config.deadFrames = frames;
        }

        configs.emplace(enemyId, config);
    }

    std::cout << "prueba loadFromFile enemyConfigLoader." << std::endl;
}

const EnemyConfig& EnemyConfigLoader::getConfig(const std::string& enemyId) const
{
    auto it = configs.find(enemyId);
    if (it == configs.end())
        throw std::runtime_error("EnemyConfig not found for: " + enemyId);

    return it->second;
}

bool EnemyConfigLoader::hasConfig(const std::string& enemyId) const
{
    return configs.find(enemyId) != configs.end();
}
