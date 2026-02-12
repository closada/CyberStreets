#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(
    const sf::Vector2f& startPos,
    const EnemyConfig& config,
    const sf::Texture& runTex,
    const sf::Texture& attackTex,
    const sf::Texture& deadTex
)
: Character(startPos)
, config(config)
, attackCooldownTimer(0.f)
{
    maxHealth = config.maxHealth;
    health    = maxHealth;
    speed     = config.speed;
    damage    = config.damage;

    // body
    body.setSize({48.f, 48.f});
    body.setOrigin(body.getSize() / 2.f);

    // animaciones
    animation.addAnimation(
        "run",
        runTex,
        config.runFrames,
        0.10f,
        true
    );

    animation.addAnimation(
        "attack",
        attackTex,
        config.attackFrames,
        0.08f,
        false
    );

    animation.addAnimation(
        "dead",
        deadTex,
        config.deadFrames,
        0.12f,
        false
    );

    animation.play("run");
    animation.setScale({3.f, 3.f});
}

// -------------------------------------------------
void Enemy::update(float dt)
{
    if (isDead())
        return;

    Character::update(dt);

    if (attackCooldownTimer > 0.f)
        attackCooldownTimer -= dt;

    updateAnimationState();
}

// -------------------------------------------------
void Enemy::updateAI(const sf::Vector2f& playerPos, float dt)
{
    if (isDead() || isAttacking())
        return;

    sf::Vector2f pos   = body.getPosition();
    sf::Vector2f delta = playerPos - pos;

    // dirección
    direction = (delta.x < 0) ? Direction::Left : Direction::Right;

    float absX = std::abs(delta.x);
    float absY = std::abs(delta.y);

    // intento de ataque
    if (absX < 50.f && absY < 40.f && attackCooldownTimer <= 0.f)
    {
        startAttack();
        attackCooldownTimer = config.attackCooldown;
        return;
    }

    // movimiento tipo persecución (TU lógica original)
    sf::Vector2f movement(0.f, 0.f);

    if (absX > 10.f)
        movement.x = (delta.x < 0 ? -1.f : 1.f);

    if (absY > 10.f)
        movement.y = (delta.y < 0 ? -0.5f : 0.5f);

    move(movement, dt);
}

// -------------------------------------------------
void Enemy::updateAnimationState()
{
    if (isDead())
    {
        animation.play("dead");
        return;
    }

    if (isAttacking())
    {
        if (animation.getCurrentAnimation() != "attack")
            animation.play("attack");

        return;
    }


    animation.play("run");
}

// -------------------------------------------------
void Enemy::takeDamage(int amount)
{
    if (!canReceiveDamage())
        return;

    Character::takeDamage(amount);
}

bool Enemy::canReceiveDamage() const
{
    return !isDead();
}


int Enemy::getDamage() const
{
    return damage;
}
