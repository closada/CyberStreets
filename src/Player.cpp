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
    // si está muerto, no hace nada
    if (state == PlayerState::DEAD)
        return;

    // --- lógica de ataque ---
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

    // --- cooldown de daño ---
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
    if (!canReceiveDamage() || state == PlayerState::DEAD)
        return;

    health -= amount;

    if (health <= 0)
    {
        health = 0;
        state = PlayerState::DEAD;
        body.setFillColor(sf::Color(80, 80, 80)); // gris = muerto
        return;
    }

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


bool Player::isDead() const
{
    return state == PlayerState::DEAD;
}

void Player::move(const sf::Vector2f& dir)
{
    if (state == PlayerState::DEAD || state == PlayerState::ATTACKING)
        return;

    body.move(dir * speed);

    if (dir.x < 0)
        direction = Direction::LEFT;
    else if (dir.x > 0)
        direction = Direction::RIGHT;

    if (dir.x != 0 || dir.y != 0)
        state = PlayerState::MOVING;
    else
        state = PlayerState::IDLE;
}


void Player::attack()
{
    if (state == PlayerState::ATTACKING)
        return;

    state = PlayerState::ATTACKING;
    attackClock.restart();
}


void Player::setAvatar(AvatarType avatar)
{
    switch (avatar)
    {
        case AvatarType::GREEN:
            body.setFillColor(sf::Color::Green);
            break;
        case AvatarType::BLUE:
            body.setFillColor(sf::Color::Blue);
            break;
        case AvatarType::RED:
            body.setFillColor(sf::Color::Red);
            break;
    }
}
