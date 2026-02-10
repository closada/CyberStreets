#pragma once
#include "Enemy.hpp"

#include <iostream>
#include <vector>
#include <string>


struct EnemyWave
{
    std::string enemyId;   // "Thug", "Boss", etc
    float x;
    float y;
    float delay;
    int count;
};


struct LevelData
{
    int id;
    float levelLength;
    float goalX;
    float groundY;

    std::vector<EnemyWave> waves;
};


class LevelLoader
{
public:
    static LevelData loadFromFile(const std::string& path);
};

