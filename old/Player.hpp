#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Avatar.hpp"


enum class Direction
{
    LEFT,
    RIGHT
};

enum class PlayerState
{
    IDLE,
    MOVING,
    //ATTACKING,
    DEAD
};

class Player : public Entity
{
private:
    static constexpr int ATTACK_FRAMES = 6;

    int health;
    Direction direction;
    PlayerState state;

    bool isAttackingPlayer;

    // ataque
    sf::RectangleShape attackBox;
    sf::Clock attackClock;
    const sf::Time attackDuration = sf::milliseconds(200);
    bool hitDone;


    // daño
    sf::Clock damageClock;
    const sf::Time damageCooldown = sf::milliseconds(500);
    bool canBeDamaged;

    AvatarType avatar;

    // Sprites
    sf::Texture idleTex;
    sf::Texture runTex;
    sf::Texture attackTex;
    sf::Texture runAttackTex;

public:
    Player(float x, float y);

    void update() override;
    void draw(sf::RenderWindow& window) override;

    void move(const sf::Vector2f& dir);
    void attack();

    void keepInside(const sf::View& view);

    void takeDamage(int amount);
    bool canReceiveDamage() const;

    int getHealth() const;

    void setColor(const sf::Color& color); // ← ESTA LÍNEA

    sf::FloatRect getAttackBounds() const;
    bool isAttacking() const;
    void drawAttack(sf::RenderWindow& window);

    bool isDead() const;

    void setAvatar(AvatarType avatar);
    void updateState();
    void updateTexture();
    void updateSprite();

    bool canHit() const { return isAttackingPlayer && !hitDone; }
    void markHit() { hitDone = true; }

protected:
    void onAnimationFinished() override;
    void updateAnimation() override;

};

#endif
