#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y)
{
    health = 10;

    body.setSize({48.f, 48.f});
    body.setFillColor(sf::Color::Red);
    body.setPosition(x, y);
    body.setOrigin(body.getSize() / 2.f);
    speed = 2.f;

    idleTex.loadFromFile("assets/sprites/enemies/1_Walk.png");
    runTex.loadFromFile("assets/sprites/enemies/1_Walk.png");
    attackTex.loadFromFile("assets/sprites/enemies/1_Attack.png");

    setTexture(runTex, 6);

    sprite.setScale(3.f, 3.f);

    attackBox.setSize({32.f, 20.f});
    attackBox.setOrigin(16.f, 10.f);

    state = EnemyState::MOVING;
    isAttackingEnemy = false;
    facingLeft = true;

    canAttack = true;
    canTakeDamage = true;

    attackBox.setSize({32.f, 20.f});
}

void Enemy::update()
{
    if (state == EnemyState::DEAD)
        return;

    // cooldown daño
    if (!canTakeDamage && damageClock.getElapsedTime() > damageCooldown)
        canTakeDamage = true;

    // HIT
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

    // ataque
    if (isAttacking())
    {
        float offsetX = facingLeft ? -32.f : 32.f;
        attackBox.setPosition(
            body.getPosition().x + offsetX,
            body.getPosition().y
        );

        if (attackClock.getElapsedTime() > attackDuration)
            isAttackingEnemy = false;
    }


    if (!canAttack && attackClock.getElapsedTime() > attackCooldown)
        canAttack = true;

    updateAnimation();
}



void Enemy::updateAI(const sf::Vector2f& playerPos)
{
    if (state == EnemyState::DEAD || state == EnemyState::HIT)
        return;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f delta = playerPos - pos;

    facingLeft = delta.x < 0;

    // intento de ataque
    if (tryAttack(playerPos))
        return;

    // movimiento
    sf::Vector2f movement(0.f, 0.f);

    if (std::abs(delta.x) > 10.f)
        movement.x = (delta.x < 0 ? -speed : speed);

    if (std::abs(delta.y) > 10.f)
        movement.y = (delta.y < 0 ? -speed * 0.5f : speed * 0.5f);

    move(movement);
    state = EnemyState::MOVING;
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
    return isAttackingEnemy;
}

sf::FloatRect Enemy::getAttackBounds() const
{
    return attackBox.getGlobalBounds();
}

void Enemy::drawAttack(sf::RenderWindow& window)
{
    if (isAttacking())
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
    if (!canAttack || isAttacking())
        return false;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f delta = playerPos - pos;

    if (std::abs(delta.x) < 50.f && std::abs(delta.y) < 30.f)
    {
        isAttackingEnemy = true;
        canAttack = false;
        attackClock.restart();

        attackBox.setPosition(
            pos.x + (facingLeft ? -32.f : 32.f),
            pos.y
        );

        setTexture(attackTex, 6);
        currentFrame = 0;
        animationClock.restart();

        return true;
    }

    return false;
}


void Enemy::draw(sf::RenderWindow& window)
{
    sprite.setScale(
        facingLeft ? -3.f : 3.f,
        3.f
    );

    sprite.setPosition(body.getPosition());
    window.draw(sprite);
}

