#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "AvatarConfigLoader.hpp"
#include "ResourceManager.hpp"

class AvatarFactory
{
public:
    AvatarFactory(
        AvatarConfigLoader& configLoader,
        ResourceManager<sf::Texture, std::string>& textures
    );

    std::unique_ptr<Player> createPlayer(
        const std::string& avatarId,
        const sf::Vector2f& position
    );

private:
    AvatarConfigLoader& configLoader;
    ResourceManager<sf::Texture, std::string>& textures;
};

