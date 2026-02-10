#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct EnemyConfig
{
    int maxHealth;
    int damage;
    float attackCooldown;
    float speed;

    std::string runTexId;
    int runFrames;

    std::string attackTexId;
    int attackFrames;

    std::string deadTexId;
    int deadFrames;
};
