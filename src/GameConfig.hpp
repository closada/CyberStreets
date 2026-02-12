#pragma once
#include <string>
#include <vector>

struct LevelEntry
{
    int id;
    std::string file;
};

struct GameConfig
{
    std::string name;
    std::string typography;
    int score;
    int lastLevelCompleted;
    std::string music;

    std::vector<LevelEntry> levels;
};

