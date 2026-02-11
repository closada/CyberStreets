#pragma once
#include <string>

struct AvatarConfig
{
    int damage;
    int maxHealth;
    float speed;

    std::string idleTexId;
    int idleFrames;
    float idleDuration;

    std::string runTexId;
    int runFrames;
    float runDuration;

    std::string attackTexId;
    int attackFrames;
    float attackDuration;

    std::string runAttackTexId;
    int runAttackFrames;
    float runAttackDuration;
};

