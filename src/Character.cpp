#include "Character.hpp"

// -------------------------------------------------
// Constructor
// -------------------------------------------------
Character::Character(const sf::Vector2f& startPos)
: Entity(startPos),
  state(CharacterState::Idle),
  direction(Direction::Right),
  health(0),
  speed(0.f),
  attacking(false),
  hitDone(false)
{
    // attack box genérico
    attackBox.setSize({32.f, 20.f});
    attackBox.setOrigin(16.f, 10.f);
}

// -------------------------------------------------
// Update
// -------------------------------------------------
void Character::update(float dt)
{
    // actualizar animación
    animation.update();

    // si está muerto no hace nada más
    if (state == CharacterState::Dead)
        return;


    // actualiza el box de ataque
    if (attacking)
    {
        updateAttackBox();
    }

    // reset automático cuando termina la animación de ataque
    if (attacking && animation.isFinished())
    {
        attacking = false;
        hitDone = false;
    }

    if (direction == Direction::Left)
        animation.setScale({-3.f, 3.f});
    else
        animation.setScale({ 3.f, 3.f});

}

// -------------------------------------------------
// Movimiento
// -------------------------------------------------
void Character::move(const sf::Vector2f& dir, float dt)
{
    if (state == CharacterState::Dead)
        return;

    if (dir.x == 0.f && dir.y == 0.f)
    {
        state = CharacterState::Idle;
        return;
    }

    body.move(dir * speed * dt);

    if (dir.x < 0) direction = Direction::Left;
    if (dir.x > 0) direction = Direction::Right;

    state = CharacterState::Moving;
}

// -------------------------------------------------
// Ataque
// -------------------------------------------------
void Character::startAttack()
{
    if (attacking || state == CharacterState::Dead)
        return;

    attacking = true;
    hitDone = false;
}

bool Character::isAttacking() const
{
    return attacking;
}

bool Character::canHit() const
{
    return attacking && !hitDone;
}

void Character::markHit()
{
    hitDone = true;
}

sf::FloatRect Character::getAttackBounds() const
{
    return attackBox.getGlobalBounds();
}

// -------------------------------------------------
// Vida
// -------------------------------------------------
void Character::takeDamage(int amount)
{
    if (state == CharacterState::Dead)
        return;

    health -= amount;

    if (health <= 0)
    {
        health = 0;
        state = CharacterState::Dead;
    }
}

int Character::getHealth() const
{
    return health;
}

bool Character::isDead() const
{
    return state == CharacterState::Dead;
}

// -------------------------------------------------
// DRAW ATTACK BOX (PARA DEBUG)
// -------------------------------------------------
void Character::drawAttackBox(sf::RenderWindow& window)
{
    if (!attacking)
        return;

    sf::RectangleShape debugBox = attackBox;
    debugBox.setFillColor(sf::Color::Transparent);
    debugBox.setOutlineColor(sf::Color::Red);
    debugBox.setOutlineThickness(1.f);

    window.draw(debugBox);
}

void Character::updateAttackBox()
{
    sf::Vector2f offset;

    float xOffset = body.getSize().x / 2.f + attackBox.getSize().x / 2.f;

    if (direction == Direction::Right)
        offset = { xOffset, 0.f };
    else
        offset = { -xOffset, 0.f };

    attackBox.setPosition(body.getPosition() + offset);
}
