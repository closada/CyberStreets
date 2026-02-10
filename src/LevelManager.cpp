#include "LevelManager.hpp"
#include <algorithm>

LevelManager::LevelManager(
    const std::string& levelPath,
    ResourceManager<sf::Texture, std::string>& enemyTextures,
    SoundManager& sounds,
    AvatarType avatar
)
: enemyTextures(enemyTextures),
  sound(sounds),
  enemyConfigLoader(this->enemyTextures),
  enemyFactory(enemyConfigLoader, this->enemyTextures),
  levelData(LevelLoader::loadFromFile(levelPath)),
  waveManager(levelData, enemyFactory, enemies)
{

    // cargar configs de enemigos
    enemyConfigLoader.loadFromFile("assets/config/enemies/enemyTypes.json");


    // --- CÁMARA ---
    camera = sf::View(sf::FloatRect(0, 0, 800, 600));
    levelRightEnd = levelData.levelLength;

    // --- GOAL ---
    levelGoal = std::make_unique<LevelGoal>(
        levelData.goalX,
        levelData.groundY,
        textures.get(TextureID::GoalFlag)
    );

    // --- PLAYER ---
    TextureID idle, run, attack, runAttack;

    switch (avatar)
    {
        case AvatarType::Biker:
            idle = TextureID::BikerIdle;
            run  = TextureID::BikerRun;
            attack = TextureID::BikerAttack;
            runAttack = TextureID::BikerRunAttack;
            break;

        case AvatarType::Cyborg:
            idle = TextureID::CyborgIdle;
            run  = TextureID::CyborgRun;
            attack = TextureID::CyborgAttack;
            runAttack = TextureID::CyborgRunAttack;
            break;

        case AvatarType::Punk:
            idle = TextureID::PunkIdle;
            run  = TextureID::PunkRun;
            attack = TextureID::PunkAttack;
            runAttack = TextureID::PunkRunAttack;
            break;
    }

    player = std::make_unique<Player>(
        sf::Vector2f(400.f, levelData.groundY),
        textures.get(idle),
        textures.get(run),
        textures.get(attack),
        textures.get(runAttack)
    );
}

void LevelManager::handleInput(const InputCommand& cmd, float dt)
{
    if (!player)
        return;

    player->handleMovement(cmd.movement, dt);

    if (cmd.attackPressed)
        player->handleAttack();
}

void LevelManager::update(float dt)
{
    // --- PLAYER ---
    player->update(dt);
    player->keepInside(camera);

    bool isAttackingNow = player->isAttacking();
    if (isAttackingNow && !wasPlayerAttacking)
        sound.play(SoundID::PLAYER_HIT);

    wasPlayerAttacking = isAttackingNow;

    // --- WAVES ---
    waveManager.update(dt);

    // --- ENEMIES ---
    for (auto& e : enemies)
    {
        e->update(dt);
        e->updateAI(player->getPosition(), dt);
    }

    // --- SEPARACIÓN ---
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            if (enemies[i]->getBounds().intersects(enemies[j]->getBounds()))
            {
                enemies[i]->move({0.f, -0.5f}, dt);
                enemies[j]->move({0.f,  0.5f}, dt);
            }
        }
    }

    // --- PLAYER → ENEMY ---
    if (player->canHit())
    {
        for (auto& e : enemies)
        {
            if (player->getAttackBounds().intersects(e->getBounds()))
            {
                e->takeDamage(10);
                player->markHit();
                sound.play(SoundID::HIT);
                break;
            }
        }
    }

    // --- ENEMY → PLAYER ---
    for (auto& e : enemies)
    {
        if (e->isAttacking() &&
            e->getAttackBounds().intersects(player->getBounds()))
        {
            if (player->canReceiveDamage())
            {
                player->takeDamage(10);
                sound.play(SoundID::PLAYER_HIT);
            }
        }
    }

    // --- CLEANUP ---
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e)
            {
                return e->isDead();
            }),
        enemies.end()
    );

    // --- GOAL ---
    levelGoal->update(*player);
}

void LevelManager::draw(sf::RenderWindow& window)
{
    window.setView(camera);

    levelGoal->draw(window);
    player->draw(window);
    player->drawAttackBox(window);

    for (auto& e : enemies)
        e->draw(window);
}


bool LevelManager::isGameOver() const
{
    return player->isDead();
}

bool LevelManager::isLevelCompleted() const
{
    return levelGoal->isReached() &&
           waveManager.isFinished() &&
           enemies.empty();
}


bool LevelManager::isPlayerDead() const
{
    return player->isDead();
}

int LevelManager::getPlayerHealth() const
{
    return player->getHealth();
}

float LevelManager::getPlayerMaxDistance() const
{
    return player->getMaxDistanceReached();
}

const sf::View& LevelManager::getCamera() const
{
    return camera;
}

