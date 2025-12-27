#include "Enemy.hpp"

Enemy::Enemy(float x, float y)
{
    body.setSize(sf::Vector2f(50.f, 80.f));
    body.setFillColor(sf::Color::Red);
    body.setPosition(x, y);
    speed = 2.f;
}

void Enemy::update()
{
    body.move(-speed, 0);
}

void Enemy::takeHit()
{
    body.move(20.f, 0);
}

