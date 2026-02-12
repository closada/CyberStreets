#pragma once
#include <string>
#include <vector>
#include <unordered_map>



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
    std::unordered_map<std::string, std::string> sounds;

};

