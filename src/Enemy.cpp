#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y)
{
    health = 10;

    body.setSize({50.f, 80.f});
    body.setFillColor(sf::Color::Red);
    body.setPosition(x, y);
    speed = 2.f;

    state = EnemyState::MOVING;
    canAttack = true;
    canTakeDamage = true; // ← NUEVO

    attackBox.setSize({40.f, 30.f});
    attackBox.setFillColor(sf::Color::Transparent);
    attackBox.setOutlineColor(sf::Color::Yellow);
    attackBox.setOutlineThickness(1.f);
}

void Enemy::update()
{
    // no hace nada sola
}

void Enemy::updateAI(const sf::Vector2f& playerPos)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f delta = playerPos - pos;

    /* REACTIVAR POSIBILIDAD DE RECIBIR HIT */
    if (!canTakeDamage && damageClock.getElapsedTime() > damageCooldown)
    {
        canTakeDamage = true;
    }


    // si es golpeado
    if (state == EnemyState::HIT)
    {
        body.move(knockbackVelocity);

        if (hitClock.getElapsedTime() > hitDuration)
        {
            state = EnemyState::MOVING;
            knockbackVelocity = {0.f, 0.f};
        }
        return;
    }


    // --- cooldown ataque ---
    if (!canAttack && attackClock.getElapsedTime() > attackCooldown)
        canAttack = true;

    // --- si está atacando ---
    if (state == EnemyState::ATTACKING)
    {
        if (attackClock.getElapsedTime() > attackDuration)
            state = EnemyState::MOVING;
        return;
    }

    float distanceX = std::abs(delta.x);

    // --- decide atacar ---
    if (distanceX < 60.f && std::abs(delta.y) < 40.f && canAttack)
    {
        state = EnemyState::ATTACKING;
        canAttack = false;
        attackClock.restart();

        attackBox.setPosition(
            pos.x + (delta.x < 0 ? -attackBox.getSize().x : body.getSize().x),
            pos.y + 20.f
        );
        return;
    }

    // --- movimiento hacia el player ---
    float moveX = 0.f;
    float moveY = 0.f;

    if (std::abs(delta.x) > 5.f)
        moveX = (delta.x < 0 ? -speed : speed);

    if (std::abs(delta.y) > 5.f)
        moveY = (delta.y < 0 ? -speed * 0.6f : speed * 0.6f);

    body.move(moveX, moveY);
}


void Enemy::takeHit(const sf::Vector2f& attackerPos)
{
    if (!canTakeDamage || state == EnemyState::DEAD)
        return;

    canTakeDamage = false;
    damageClock.restart();

    state = EnemyState::HIT;
    hitClock.restart();

    float dir = (body.getPosition().x < attackerPos.x) ? -1.f : 1.f;
    knockbackVelocity = { dir * 4.f, -1.f };

    health--;

    if (health <= 0)
    {
        state = EnemyState::DEAD;
    }
}


bool Enemy::isAttacking() const
{
    return state == EnemyState::ATTACKING;
}

sf::FloatRect Enemy::getAttackBounds() const
{
    return attackBox.getGlobalBounds();
}

void Enemy::drawAttack(sf::RenderWindow& window)
{
    if (state == EnemyState::ATTACKING)
        window.draw(attackBox);
}

int Enemy::getHealth() const
{
    return health;
}
