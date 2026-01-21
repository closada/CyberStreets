#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Avatar.hpp"


enum class Direction
{
    LEFT,
    RIGHT
};

enum class PlayerState
{
    IDLE,
    MOVING,
    ATTACKING,
    DEAD
};

class Player : public Entity
{
private:
    int health;
    Direction direction;
    PlayerState state;

    // ataque
    sf::RectangleShape attackBox;
    sf::Clock attackClock;
    const sf::Time attackDuration = sf::milliseconds(200);


    // daño
    sf::Clock damageClock;
    const sf::Time damageCooldown = sf::milliseconds(500);
    bool canBeDamaged;

    AvatarType avatar;

public:
    Player(float x, float y);

    void update() override;
    void move(const sf::Vector2f& dir);
    void attack();

    void keepInside(const sf::Vector2u& windowSize);

    void takeDamage(int amount);
    bool canReceiveDamage() const;

    int getHealth() const;

    void setColor(const sf::Color& color); // ← ESTA LÍNEA

    sf::FloatRect getAttackBounds() const;
    bool isAttacking() const;
    void drawAttack(sf::RenderWindow& window);

    bool isDead() const;

    void setAvatar(AvatarType avatar);



};

#endif
