#pragma once
#include "Character.hpp"

class Player : public Character
{
public:
    Player(
        const sf::Vector2f& startPos,
        const sf::Texture& idleTex,
        const sf::Texture& runTex,
        const sf::Texture& attackTex,
        const sf::Texture& runAttackTex
    );

    // --- core ---
    void update(float dt) override;

    // --- input ---
    void handleMovement(const sf::Vector2f& dir, float dt);
    void handleAttack();

    // --- límites ---
    void keepInside(const sf::View& view);

private:
    // helpers internos
    void updateAnimationState();

    // métricas
    float maxDistanceReached;
};

