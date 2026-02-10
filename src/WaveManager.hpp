#pragma once
#include "EnemyFactory.hpp"
#include "LevelLoader.hpp"
#include <vector>
#include <memory>


struct WaveState
{
    EnemyWave data;
    float timer = 0.f;
    int spawned = 0;
    bool finished = false;
};


class WaveManager
{
public:
    WaveManager(
        const LevelData& levelData,
        EnemyFactory& enemyFactory,
        std::vector<std::unique_ptr<Enemy>>& enemies
    );

    void update(float dt);
    bool isFinished() const;

private:
    EnemyFactory& enemyFactory;
    std::vector<std::unique_ptr<Enemy>>& enemies;

    std::vector<WaveState> waves;
};

