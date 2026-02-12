#pragma once

#include "Character.hpp"
#include "EnemyConfig.hpp"
#include <SFML/System/Vector2.hpp>

class Enemy : public Character
{
public:
    Enemy(
        const sf::Vector2f& startPos,
        const EnemyConfig& config,
        const sf::Texture& runTex,
        const sf::Texture& attackTex,
        const sf::Texture& deadTex
    );

    void update(float dt) override;
    void updateAI(const sf::Vector2f& playerPos, float dt);

    // combate
    int getDamage() const;

    void takeDamage(int amount);
    bool canReceiveDamage() const;

private:
    void updateAnimationState();

private:
    EnemyConfig config;

    float attackCooldownTimer;
};
