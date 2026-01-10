#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

enum class EnemyState
{
    MOVING,
    ATTACKING,
    HIT,
    DEAD
};

class Enemy : public Entity
{
private:
    EnemyState state;
    int health;

    // ataque
    sf::RectangleShape attackBox;
    sf::Clock attackClock;
    sf::Time attackDuration = sf::milliseconds(250);
    sf::Time attackCooldown = sf::milliseconds(800);
    bool canAttack;

    // GOLPE
    sf::Vector2f knockbackVelocity;
    sf::Clock hitClock;
    sf::Time hitDuration = sf::milliseconds(200);

    // HIT
    sf::Clock damageClock;
    sf::Time damageCooldown = sf::milliseconds(300);
    bool canTakeDamage;



public:
    Enemy(float x, float y);

    void update() override; // ← OBLIGATORIO
    void updateAI(const sf::Vector2f& playerPos); // ← NUEVO

    void takeHit(const sf::Vector2f& attackerPos);

    bool isAttacking() const;
    sf::FloatRect getAttackBounds() const;
    void drawAttack(sf::RenderWindow& window);

    int getHealth() const;

    bool isDead() const { return state == EnemyState::DEAD; }

};

#endif
