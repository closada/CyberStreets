#include "EnemyFactory.hpp"

EnemyFactory::EnemyFactory(
    EnemyConfigLoader& configLoader,
    ResourceManager<sf::Texture, std::string>& textures
)
: configLoader(configLoader)
, textures(textures)
{
}

std::unique_ptr<Enemy> EnemyFactory::createEnemy(
    const std::string& enemyId,
    const sf::Vector2f& position
)
{
    if (!configLoader.hasConfig(enemyId))
        throw std::runtime_error("EnemyFactory: unknown enemyId: " + enemyId);

    const EnemyConfig& cfg = configLoader.getConfig(enemyId);

    return std::make_unique<Enemy>(
        position,
        cfg,
        textures.get(cfg.runTexId),
        textures.get(cfg.attackTexId),
        textures.get(cfg.deadTexId)
    );
}
