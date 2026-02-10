#include "WaveManager.hpp"

WaveManager::WaveManager(
    const LevelData& levelData,
    EnemyFactory& enemyFactory,
    std::vector<std::unique_ptr<Enemy>>& enemies
)
: enemyFactory(enemyFactory),
  enemies(enemies)
{
    for (const auto& w : levelData.waves)
    {
        WaveState state;
        state.data = w;
        waves.push_back(state);
    }
}

void WaveManager::update(float dt)
{
    for (auto& wave : waves)
    {
        if (wave.finished)
            continue;

        wave.timer += dt;

        if (wave.timer >= wave.data.delay &&
            wave.spawned < wave.data.count)
        {
            auto enemy = enemyFactory.createEnemy(
                wave.data.enemyId,
                { wave.data.x, wave.data.y }
            );

            enemies.push_back(std::move(enemy));
            wave.spawned++;

            wave.timer = 0.f;
        }

        if (wave.spawned >= wave.data.count)
            wave.finished = true;
    }
}

bool WaveManager::isFinished() const
{
    for (const auto& w : waves)
        if (!w.finished)
            return false;

    return true;
}

