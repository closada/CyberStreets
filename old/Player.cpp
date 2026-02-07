#include "Player.hpp"

Player::Player(float x, float y)
{
    health = 100;
    direction = Direction::RIGHT;
    state = PlayerState::IDLE;
    isAttackingPlayer = false;
    hitDone = false;

    body.setSize({48.f, 48.f});
    body.setOrigin(body.getSize() / 2.f);
    body.setPosition(x, y);

    speed = 4.f;

    setAvatar(AvatarType::GREEN);
    setTexture(idleTex, 4);

    sprite.setScale(3.f, 3.f);

    canBeDamaged = true;


    attackBox.setSize({32.f, 20.f});
    attackBox.setOrigin(16.f, 10.f);
    attackBox.setFillColor(sf::Color::Transparent);
    attackBox.setOutlineColor(sf::Color::Green);
    attackBox.setOutlineThickness(1.f);
}


void Player::update()
{
    if (state == PlayerState::DEAD)
        return;

    // --- ATAQUE ---
    if (isAttackingPlayer)
    {
        if (attackClock.getElapsedTime() > attackDuration)
        {
            //isAttackingPlayer = false;
            hitDone = true;
        }

            float offsetX = (direction == Direction::RIGHT) ? 32.f : -32.f;
            attackBox.setPosition(
                body.getPosition().x + offsetX,
                body.getPosition().y
            );

    }

    // --- cooldown daño ---
    if (!canBeDamaged && damageClock.getElapsedTime() > damageCooldown)
        canBeDamaged = true;

    updateSprite();
    updateAnimation();
}



void Player::draw(sf::RenderWindow& window)
{
    sprite.setScale(
        direction == Direction::LEFT ? -3.f : 3.f,
        3.f
    );

    sprite.setPosition(body.getPosition());
    window.draw(sprite);
}




void Player::keepInside(const sf::View& view)
{
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f half = body.getSize() / 2.f;

    sf::Vector2f vc = view.getCenter();
    sf::Vector2f vs = view.getSize();

    float left   = vc.x - vs.x / 2.f + half.x;
    float right  = vc.x + vs.x / 2.f - half.x;
    float top    = vc.y - vs.y / 2.f + half.y;
    float bottom = vc.y + vs.y / 2.f - half.y;

    if (pos.x < left)   pos.x = left;
    if (pos.x > right)  pos.x = right;

    float minY = top + vs.y * 0.35f;
    if (pos.y < minY)   pos.y = minY;
    if (pos.y > bottom) pos.y = bottom;

    body.setPosition(pos);
}





bool Player::canReceiveDamage() const
{
    return !isAttacking() && canBeDamaged;
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
    return isAttackingPlayer;
}


void Player::drawAttack(sf::RenderWindow& window)
{
    if (isAttacking())
        window.draw(attackBox);
}


bool Player::isDead() const
{
    return state == PlayerState::DEAD;
}

void Player::move(const sf::Vector2f& dir)
{
    if (state == PlayerState::DEAD)
        return;

    body.move(dir * speed);

    if (dir.x < 0) direction = Direction::LEFT;
    else if (dir.x > 0) direction = Direction::RIGHT;

    if (dir.x != 0 || dir.y != 0)
        state = PlayerState::MOVING;
    else
        state = PlayerState::IDLE;
}



void Player::attack()
{
    if (isAttackingPlayer)
        return;

    isAttackingPlayer = true;
    hitDone = false;
    frameTime = attackDuration.asSeconds() / ATTACK_FRAMES;

    currentFrame = 0;

    attackClock.restart();

    animationClock.restart();
}




void Player::setAvatar(AvatarType avatar)
{
    std::string basePath;

    switch (avatar)
    {
        case AvatarType::GREEN: basePath = "assets/sprites/player/Biker_"; break;
        case AvatarType::BLUE:  basePath = "assets/sprites/player/Cyborg_"; break;
        case AvatarType::RED:   basePath = "assets/sprites/player/Punk_"; break;
    }

    idleTex.loadFromFile(basePath + "idle.png");
    runTex.loadFromFile(basePath + "run.png");
    attackTex.loadFromFile(basePath + "attack.png");
    runAttackTex.loadFromFile(basePath + "run_attack.png");

    setTexture(idleTex, 4);
}


void Player::updateSprite()
{
    static PlayerState lastState = PlayerState::IDLE;
    static bool lastAttack = false;

    if (state == lastState && isAttackingPlayer == lastAttack)
        return;

    currentFrame = 0;
    animationClock.restart();

    if (isAttackingPlayer)
    {

        frameTime = 0.11f;

        if (state == PlayerState::MOVING)
            setTexture(runAttackTex, 6);
        else
            setTexture(attackTex, 6);
    }
    else
    {
        if (state == PlayerState::MOVING)
        {
             frameTime = 0.10f;
             setTexture(runTex, 6);
        }

        else
        {
            frameTime = 0.14f;
            setTexture(idleTex, 4);
        }

    }

    lastState = state;
    lastAttack = isAttackingPlayer;
}


void Player::onAnimationFinished()
{
    if (isAttackingPlayer)
    {
        isAttackingPlayer = false;
        frameTime = 0.12f;   // vuelve a normal
        currentFrame = 0;
    }
}

void Player::updateAnimation()
{
    if (animationClock.getElapsedTime().asSeconds() >= frameTime)
    {
        animationClock.restart();
        currentFrame++;

        if (isAttackingPlayer)
        {
            // ATAQUE: NO LOOP
            if (currentFrame >= frameCount)
            {
                currentFrame = frameCount - 1;
                onAnimationFinished();
                return;
            }
        }
        else
        {
            // IDLE / RUN: LOOP NORMAL
            if (currentFrame >= frameCount)
                currentFrame = 0;
        }

        sprite.setTextureRect(sf::IntRect(
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight
        ));
    }
}

