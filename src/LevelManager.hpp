#pragma once
#include "LevelLoader.hpp"
#include "WaveManager.hpp"
#include "Player.hpp"
#include "ResourceAliases.hpp"
#include "LevelGoal.hpp"
#include "InputController.hpp"
#include "SoundManager.hpp"
#include "EnemyConfigLoader.hpp"
#include "EnemyFactory.hpp"
#include "ResourceManager.hpp"
#include "AvatarConfigLoader.hpp"
#include "AvatarFactory.hpp"
#include "AvatarConfig.hpp"



class LevelManager
{
public:
    LevelManager(
        const std::string& levelPath,
        ResourceManager<sf::Texture, std::string>& textures,
        SoundManager& sounds,
        const std::string& avatarId
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


    // refs
    ResourceManager<sf::Texture, std::string>& textures;
    SoundManager& sound;

    // nivel
    LevelData levelData;


    EnemyConfigLoader enemyConfigLoader;
    AvatarConfigLoader avatarConfigLoader;


    EnemyFactory enemyFactory;
    AvatarFactory avatarFactory;


    // core
    std::unique_ptr<Player> player;
    std::unique_ptr<LevelGoal> levelGoal;

    std::vector<std::unique_ptr<Enemy>> enemies;
    WaveManager waveManager;

    // cámara
    sf::View camera;
    float levelRightEnd;
    bool wasPlayerAttacking = false;



};

