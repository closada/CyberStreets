#include "Player.hpp"

Player::Player(float x, float y)
{
    health = 100;
    direction = RIGHT;


    body.setSize(sf::Vector2f(50.f, 80.f));
    body.setFillColor(sf::Color::Green);
    body.setPosition(x, y);
    speed = 4.f;


    /* configuraciones iniciales de ataque */
    attacking = false;

    attackBox.setSize(sf::Vector2f(40.f, 30.f));
    attackBox.setFillColor(sf::Color::Transparent);
    attackBox.setOutlineColor(sf::Color::White);
    attackBox.setOutlineThickness(1.f);

}

void Player::update()
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= speed;
        direction = LEFT;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x = speed;
        direction = RIGHT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed;

    body.move(movement);

    /* si se quiere atacar */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && !attacking)
    {
        attacking = true;
        attackClock.restart();
    }

    /* si esta atacando */
    if (attacking)
    {
        /* tiempo */
        if (attackClock.getElapsedTime().asMilliseconds() > 200)
        {
            attacking = false;
        }

        /* se agrega objeto que muestra visualmente el "golpe" */
        sf::Vector2f pos = body.getPosition();
        if (direction == RIGHT)
        {
            attackBox.setPosition(
                pos.x + body.getSize().x,
                pos.y + 20.f
            );
        }
        else // LEFT
        {
            attackBox.setPosition(
                pos.x - attackBox.getSize().x,
                pos.y + 20.f
            );
        }

    }


}

void Player::move(float x, float y)
{
    body.move(x, y);
}

void Player::keepInside(const sf::Vector2u& windowSize)
{
    sf::FloatRect bounds = body.getGlobalBounds();

    if (bounds.left < 0)
        body.setPosition(0, bounds.top);

    if (bounds.left + bounds.width > windowSize.x)
        body.setPosition(windowSize.x - bounds.width, bounds.top);

    if (bounds.top < 0)
        body.setPosition(bounds.left, 0);

    if (bounds.top + bounds.height > windowSize.y)
        body.setPosition(bounds.left, windowSize.y - bounds.height);
}

void Player::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
        health = 0;
}

int Player::getHealth() const
{
    return health;
}

void Player::setColor(const sf::Color& color)
{
    body.setFillColor(color);
}


bool Player::isAttacking() const
{
    return attacking;
}

sf::FloatRect Player::getAttackBounds() const
{
    return attackBox.getGlobalBounds();
}

void Player::drawAttack(sf::RenderWindow& window)
{
    if (attacking)
        window.draw(attackBox);
}
