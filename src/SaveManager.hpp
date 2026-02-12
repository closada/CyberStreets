#pragma once
#include <string>
#include "GameConfig.hpp"

class SaveManager
{
public:
    static void saveProgress(const std::string& path, const GameConfig& config);
};

