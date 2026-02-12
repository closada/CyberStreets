#pragma once

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

#include "AvatarConfig.hpp"
#include "ResourceManager.hpp"

class AvatarConfigLoader
{
public:
    AvatarConfigLoader(
        ResourceManager<sf::Texture, std::string>& textures
    );

    void loadFromFile(const std::string& path);

    const AvatarConfig& getConfig(const std::string& avatarId) const;
    bool hasConfig(const std::string& avatarId) const;

    const std::unordered_map<std::string, AvatarConfig>& getAll() const;

private:
    std::unordered_map<std::string, AvatarConfig> configs;
    ResourceManager<sf::Texture, std::string>& textures;
};
