#pragma once
#include "LevelLoader.hpp"
#include "WaveManager.hpp"
#include "Player.hpp"
#include "ResourceAliases.hpp"
#include "Avatar.hpp"
#include "LevelGoal.hpp"
#include "InputController.hpp"
#include "SoundManager.hpp"
#include "EnemyConfigLoader.hpp"
#include "EnemyFactory.hpp"
#include "ResourceManager.hpp"


class LevelManager
{
public:
    LevelManager(
        const std::string& levelPath,
        TextureManager& textures,
        SoundManager& sounds,
        AvatarType avatar
    );

    void handleInput(const InputCommand& cmd, float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isPlayerDead() const;


    bool isGameOver() const;
    bool isLevelCompleted() const;

    int getPlayerHealth() const;
    float getPlayerMaxDistance() const;
    const sf::View& getCamera() const;

private:


    ResourceManager<sf::Texture, std::string>& enemyTextures;

    TextureManager& textures;
    EnemyFactory enemyFactory;
    EnemyConfigLoader enemyConfigLoader;

    // nivel
    LevelData levelData;

    // core
    std::unique_ptr<Player> player;
    std::unique_ptr<LevelGoal> levelGoal;

    std::vector<std::unique_ptr<Enemy>> enemies;
    WaveManager waveManager;

    // cámara
    sf::View camera;
    float levelRightEnd;
    bool wasPlayerAttacking = false;

    // refs
    SoundManager& sound;
};

