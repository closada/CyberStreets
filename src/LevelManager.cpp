#include "LevelManager.hpp"
#include <algorithm>
#include <iostream>

LevelManager::LevelManager(
    const std::string& levelPath,
    ResourceManager<sf::Texture, std::string>& textures,
    SoundManager& sounds,
    const std::string& avatarId
)

: textures(textures),
  sound(sounds),
  levelData(LevelLoader::loadFromFile(levelPath)),
  enemyConfigLoader(textures),
  avatarConfigLoader(textures),
  enemyFactory(enemyConfigLoader, textures),
  avatarFactory(avatarConfigLoader, textures),
  waveManager(levelData, enemyFactory, enemies)
{

    std::cout << "se ingreso al constructor de levelManager." << std::endl;

    // cargar configs de enemigos
    enemyConfigLoader.loadFromFile("assets/config/enemyTypes.json");

    std::cout << "Listo loader de enemyConfigLoader" << std::endl;

    // cargar configs de avatares
    avatarConfigLoader.loadFromFile("assets/config/Avatars.json");

    std::cout << "Listo loader de avatarConfigLoader" << std::endl;

    // --- CÁMARA ---
    camera = sf::View(sf::FloatRect(0, 0, 800, 600));
    levelRightEnd = levelData.levelLength;

    // --- GOAL ---
    levelGoal = std::make_unique<LevelGoal>(
        levelData.goalX,
        levelData.groundY,
        textures.get("goalFlag")
    );

    // --- PLAYER ---
    player = avatarFactory.createPlayer(
        avatarId,
        sf::Vector2f(400.f, levelData.groundY)
    );

    std::cout << "Constructor LevelManager Finaliza correctamente!" << std::endl;

}

void LevelManager::handleInput(const InputCommand& cmd, float dt)
{
    if (!player)
        return;

    player->handleMovement(cmd.movement, dt);

    if (cmd.attackPressed)
    {
        player->handleAttack();
    }


}

void LevelManager::update(float dt)
{
    // --- PLAYER ---
    player->update(dt);


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
                std::cout << "el player ataca a enemigo: " << player->getDamage() << std::endl;
                e->takeDamage(player->getDamage());
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
                std::cout << "el player puede recibir daño: " << e->getDamage() << std::endl;
                player->takeDamage(e->getDamage());
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


    // --- CAMERA ---
    sf::Vector2f camPos = camera.getCenter();

    // dead zone horizontal
    float deadZone = 120.f;

    float dx = player->getPosition().x - camPos.x;

    if (dx > deadZone)
    {
        camPos.x += dx - deadZone;
    }

    // límite derecho
    float halfView = camera.getSize().x / 2.f;
    float maxCamX = levelRightEnd + 20.f - halfView;

    if (camPos.x > maxCamX)
    {
        camPos.x = maxCamX;
    }

    camera.setCenter(camPos);

    // ACTUALIZAMOS PLAYER POST ACTUALIZACION CAMARA
    player->keepInside(camera);

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

