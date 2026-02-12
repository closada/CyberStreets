#pragma once
#include "Character.hpp"
#include "AvatarConfig.hpp"

class Player : public Character
{
public:
    Player(
        const sf::Vector2f& startPos,
        const AvatarConfig& config,
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

    float getMaxDistanceReached();

    // logica para HIT y muerte
    void takeDamage(int amount);

    bool canReceiveDamage() const;

    void stopAttack();


private:
    // helpers internos
    void updateAnimationState();

    // métricas
    float maxDistanceReached;


    float damageCooldown;        // tiempo entre golpes
    float damageCooldownTimer;   // contador

};

