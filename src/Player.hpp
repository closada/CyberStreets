#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

enum Direction
{
    LEFT,
    RIGHT
};

class Player : public Entity
{
private:
    int health;
    Direction direction;


public:
    Player(float x, float y);

    void update() override;
    void move(float x, float y);
    void keepInside(const sf::Vector2u& windowSize);

    void takeDamage(int amount);
    int getHealth() const;
    void setColor(const sf::Color& color);

    /* de ataque */
    bool attacking;
    sf::RectangleShape attackBox;
    sf::Clock attackClock;

    bool isAttacking() const;
    sf::FloatRect getAttackBounds() const;
    void drawAttack(sf::RenderWindow& window);



};


#endif
