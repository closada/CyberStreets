#pragma once
#include "GameConfig.hpp"

class GameConfigLoader
{
public:
    static GameConfig loadFromFile(const std::string& path);
};

