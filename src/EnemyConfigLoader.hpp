#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "EnemyConfig.hpp"
#include "ResourceManager.hpp"

class EnemyConfigLoader
{
public:
    EnemyConfigLoader(
        ResourceManager<sf::Texture, std::string>& enemyTextures
    );

    void loadFromFile(const std::string& path);

    const EnemyConfig& getConfig(const std::string& enemyId) const;

    bool hasConfig(const std::string& enemyId) const;

private:
    std::map<std::string, EnemyConfig> configs;
    ResourceManager<sf::Texture, std::string>& textures;
};
