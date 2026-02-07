#include "Player.hpp"

#include <iostream>

// -------------------------------------------------
// Constructor
// -------------------------------------------------
Player::Player(const sf::Vector2f& startPos,
    const sf::Texture& idleTex,
    const sf::Texture& runTex,
    const sf::Texture& attackTex,
    const sf::Texture& runAttackTex
    )
: Character(startPos),
  maxDistanceReached(0.f)
{
    // stats del player
    health = 100;
    speed  = 240.f;

    // tamaño del body (colisión)
    body.setSize({48.f, 48.f});
    body.setOrigin(body.getSize() / 2.f);

    // animaciones
    animation.addAnimation("idle", idleTex, 4, 0.14f, true);
    animation.addAnimation("run", runTex, 6, 0.10f, true);
    animation.addAnimation("attack", attackTex, 6, 0.06f, false);
    animation.addAnimation("run_attack", runAttackTex, 6, 0.11f, false);


    animation.play("idle");

    animation.setScale({3.f, 3.f});
}

// -------------------------------------------------
// Update
// -------------------------------------------------
void Player::update(float dt)
{
    Character::update(dt);

    updateAnimationState();


    // DEBUG
    std::cout
        << "[Player] state= "
        << (state == CharacterState::Idle ? "Idle" :
            state == CharacterState::Moving ? "Moving" : "Dead")
        << " // attacking= " << (isAttacking() ? "YES" : "NO")
        << " // animation= " << getCurrentAnimation()
        << std::endl;


    // métrica de distancia (solo crece)
    float x = getPosition().x;
    if (x > maxDistanceReached)
        maxDistanceReached = x;
}

// -------------------------------------------------
// Input
// -------------------------------------------------
void Player::handleMovement(const sf::Vector2f& dir, float dt)
{
    move(dir, dt);
}

void Player::handleAttack()
{
    if (!isAttacking())
    {
        startAttack();
        //animation.play("attack", true);
    }
}

// -------------------------------------------------
// Animaciones según estado
// -------------------------------------------------
void Player::updateAnimationState()
{
    if (state == CharacterState::Dead)
    {
        animation.play("dead");
        return;
    }

    if (isAttacking())
    {
        if (state == CharacterState::Moving)
            animation.play("run_attack");
        else
            animation.play("attack");

        return;
    }


    if (state == CharacterState::Moving)
        animation.play("run");
    else
        animation.play("idle");
}

// -------------------------------------------------
// Límites de pantalla / cámara
// -------------------------------------------------
void Player::keepInside(const sf::View& view)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f half = body.getSize() / 2.f;

    sf::Vector2f vc = view.getCenter();
    sf::Vector2f vs = view.getSize();

    float left   = vc.x - vs.x / 2.f + half.x;
    float right  = vc.x + vs.x / 2.f - half.x;
    float top    = vc.y - vs.y / 2.f + half.y;
    float bottom = vc.y + vs.y / 2.f - ( 3.3f * half.y);

    // límite vertical superior (solo hasta 3/4 de la pantalla)
    float minY = top + vs.y * 0.35f;

    if (pos.x < left)   pos.x = left;
    if (pos.x > right)  pos.x = right;
    if (pos.y < minY)   pos.y = minY;
    if (pos.y > bottom) pos.y = bottom;

    body.setPosition(pos);
}

