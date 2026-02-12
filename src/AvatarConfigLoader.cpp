#include "AvatarConfigLoader.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

AvatarConfigLoader::AvatarConfigLoader(
    ResourceManager<sf::Texture, std::string>& textures
)
: textures(textures)
{
}

void AvatarConfigLoader::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open avatar.json");

    json data;
    file >> data;

    for (auto& [avatarId, value] : data.items())
    {
        AvatarConfig config;

        config.damage = value["damage"];
        config.maxHealth = value["maxHealth"];
        config.speed = value["speed"];
        config.avatarImg = value["avatar"];

        // se carga la textura
        textures.load("avatarImg." + avatarId, config.avatarImg);

        // ---------- IDLE ----------
        {
            std::string path = value["sprite_idle"]["link"];
            int frames = value["sprite_idle"]["cant_sprites"];
            float duration = value["sprite_idle"]["duration_frames"];

            std::string texId = "avatar." + avatarId + ".idle";
            textures.load(texId, path);

            config.idleTexId = texId;
            config.idleFrames = frames;
            config.idleDuration = duration;
        }

        // ---------- RUN ----------
        {
            std::string path = value["sprite_run"]["link"];
            int frames = value["sprite_run"]["cant_sprites"];
            float duration = value["sprite_idle"]["duration_frames"];

            std::string texId = "avatar." + avatarId + ".run";
            textures.load(texId, path);

            config.runTexId = texId;
            config.runFrames = frames;
            config.runDuration = duration;
        }

        // ---------- ATTACK ----------
        {
            std::string path = value["sprite_attack"]["link"];
            int frames = value["sprite_attack"]["cant_sprites"];
            float duration = value["sprite_idle"]["duration_frames"];

            std::string texId = "avatar." + avatarId + ".attack";
            textures.load(texId, path);

            config.attackTexId = texId;
            config.attackFrames = frames;
            config.attackDuration = duration;
        }

        // ---------- RUN ATTACK ----------
        {
            std::string path = value["sprite_run_attack"]["link"];
            int frames = value["sprite_run_attack"]["cant_sprites"];
            float duration = value["sprite_idle"]["duration_frames"];

            std::string texId = "avatar." + avatarId + ".run_attack";
            textures.load(texId, path);

            config.runAttackTexId = texId;
            config.runAttackFrames = frames;
            config.runAttackDuration = duration;
        }

        configs.emplace(avatarId, config);
    }
}

const AvatarConfig& AvatarConfigLoader::getConfig(const std::string& avatarId) const
{
    auto it = configs.find(avatarId);
    if (it == configs.end())
        throw std::runtime_error("AvatarConfig not found: " + avatarId);

    return it->second;
}

bool AvatarConfigLoader::hasConfig(const std::string& avatarId) const
{
    return configs.find(avatarId) != configs.end();
}

