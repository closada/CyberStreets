#include "AvatarFactory.hpp"

AvatarFactory::AvatarFactory(
    AvatarConfigLoader& configLoader,
    ResourceManager<sf::Texture, std::string>& textures
)
: configLoader(configLoader)
, textures(textures)
{
}

std::unique_ptr<Player> AvatarFactory::createPlayer(
    const std::string& avatarId,
    const sf::Vector2f& position
)
{
    if (!configLoader.hasConfig(avatarId))
        throw std::runtime_error("Unknown avatarId: " + avatarId);

    const AvatarConfig& cfg = configLoader.getConfig(avatarId);

    return std::make_unique<Player>(
        position,
        cfg,
        textures.get(cfg.idleTexId),
        textures.get(cfg.runTexId),
        textures.get(cfg.attackTexId),
        textures.get(cfg.runAttackTexId)
    );
}

