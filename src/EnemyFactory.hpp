#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Enemy.hpp"
#include "EnemyConfigLoader.hpp"
#include "ResourceManager.hpp"

class EnemyFactory
{
public:
    EnemyFactory(
        EnemyConfigLoader& configLoader,
        ResourceManager<sf::Texture, std::string>& textures
    );

    std::unique_ptr<Enemy> createEnemy(
        const std::string& enemyId,
        const sf::Vector2f& position
    );

private:
    EnemyConfigLoader& configLoader;
    ResourceManager<sf::Texture, std::string>& textures;
};
