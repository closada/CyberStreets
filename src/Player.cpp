#include "Player.hpp"

Player::Player(float x, float y)
{
    health = 100;
    direction = Direction::RIGHT;
    state = PlayerState::IDLE;

    body.setSize({50.f, 80.f});
    body.setFillColor(sf::Color::Green);
    body.setPosition(x, y);
    speed = 4.f;

    attackBox.setSize({40.f, 30.f});
    attackBox.setFillColor(sf::Color::Transparent);
    attackBox.setOutlineColor(sf::Color::White);
    attackBox.setOutlineThickness(1.f);

    /* cooldown de daño */
    canBeDamaged = true;

}

void Player::update()
{
    sf::Vector2f movement(0.f, 0.f);

    // movimiento
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= speed;
        direction = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += speed;
        direction = Direction::RIGHT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed;

    body.move(movement);

    // estado movimiento
    if (movement.x != 0 || movement.y != 0)
        state = PlayerState::MOVING;
    else if (state != PlayerState::ATTACKING)
        state = PlayerState::IDLE;

    // ataque (SPACE)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
        state != PlayerState::ATTACKING)
    {
        state = PlayerState::ATTACKING;
        attackClock.restart();
    }

    // lógica de ataque
    if (state == PlayerState::ATTACKING)
    {
        if (attackClock.getElapsedTime() > attackDuration)
        {
            state = PlayerState::IDLE;
        }

        sf::Vector2f pos = body.getPosition();

        if (direction == Direction::RIGHT)
        {
            attackBox.setPosition(
                pos.x + body.getSize().x,
                pos.y + 20.f
            );
        }
        else
        {
            attackBox.setPosition(
                pos.x - attackBox.getSize().x,
                pos.y + 20.f
            );
        }
    }

    /* para el cooldown del ataque */
    if (!canBeDamaged && damageClock.getElapsedTime() > damageCooldown)
    {
        canBeDamaged = true;
    }

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

bool Player::canReceiveDamage() const
{
    return state != PlayerState::ATTACKING && canBeDamaged;
}


void Player::takeDamage(int amount)
{
    if (!canReceiveDamage())
        return;

    health -= amount;
    if (health < 0)
        health = 0;

    canBeDamaged = false;
    damageClock.restart();
}


void Player::setColor(const sf::Color& color)
{
    body.setFillColor(color);
}


int Player::getHealth() const
{
    return health;
}



sf::FloatRect Player::getAttackBounds() const
{
    return attackBox.getGlobalBounds();
}

bool Player::isAttacking() const
{
    return state == PlayerState::ATTACKING;
}

void Player::drawAttack(sf::RenderWindow& window)
{
    if (state == PlayerState::ATTACKING)
        window.draw(attackBox);
}
