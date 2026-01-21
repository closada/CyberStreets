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
    if (state == EnemyState::DEAD)
        return;

    // cooldown daño
    if (!canTakeDamage && damageClock.getElapsedTime() > damageCooldown)
        canTakeDamage = true;

    // estado HIT
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

    // cooldown ataque
    if (!canAttack && attackClock.getElapsedTime() > attackCooldown)
        canAttack = true;

    // estado ATTACKING
    if (state == EnemyState::ATTACKING)
    {
        if (attackClock.getElapsedTime() > attackDuration)
            state = EnemyState::MOVING;
    }
}


void Enemy::updateAI(const sf::Vector2f& playerPos)
{
    if (state != EnemyState::MOVING)
        return;

    // intenta atacar
    if (tryAttack(playerPos))
        return;

    // si no ataca, se mueve
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f delta = playerPos - pos;
    sf::Vector2f dir(0.f, 0.f);

    // --- movimiento hacia el player ---
    sf::Vector2f movement(0.f, 0.f);

    if (std::abs(delta.x) > 5.f)
        movement.x = (delta.x < 0 ? -speed : speed);

    if (std::abs(delta.y) > 5.f)
        movement.y = (delta.y < 0 ? -speed * 0.6f : speed * 0.6f);

    move(movement);

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
        state = EnemyState::DEAD;
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

void Enemy::move(const sf::Vector2f& dir)
{
    if (state == EnemyState::DEAD)
        return;

    body.move(dir);
}


bool Enemy::tryAttack(const sf::Vector2f& playerPos)
{
    if (!canAttack || state != EnemyState::MOVING)
        return false;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f delta = playerPos - pos;

    if (std::abs(delta.x) < 60.f && std::abs(delta.y) < 40.f)
    {
        state = EnemyState::ATTACKING;
        canAttack = false;
        attackClock.restart();

        attackBox.setPosition(
            pos.x + (delta.x < 0 ? -attackBox.getSize().x : body.getSize().x),
            pos.y + 20.f
        );
        return true;
    }

    return false;
}

