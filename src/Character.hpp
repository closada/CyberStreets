#pragma once
#include "Entity.hpp"

enum class CharacterState
{
    Idle,
    Moving,
    Dead
};

enum class Direction
{
    Left,
    Right
};

class Character : public Entity
{
public:
    virtual ~Character() = default;

    // --- core ---
    virtual void update(float dt) override;

    // --- movimiento ---
    void move(const sf::Vector2f& dir, float dt);

    // --- ataque ---
    void startAttack();
    bool isAttacking() const;

    bool canHit() const;
    void markHit();

    sf::FloatRect getAttackBounds() const;

    // --- vida ---
    void takeDamage(int amount);
    int  getHealth() const;
    bool isDead() const;

    // para debug de ataque
    void drawAttackBox(sf::RenderWindow& window);
    void updateAttackBox();

protected:
    Character(const sf::Vector2f& startPos);

    // estado
    CharacterState state;
    Direction direction;

    // stats
    int health;
    float speed;

    // ataque
    bool attacking;
    bool hitDone;

    sf::RectangleShape attackBox;
};
