#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity
{
protected:
    sf::RectangleShape body;
    float speed = 0.f;

    // Visual
    sf::Sprite sprite;
    sf::Texture texture;

    // Animación
    int frameWidth = 48;
    int frameHeight = 48;

    int frameCount = 1;
    int currentFrame = 0;
    int currentFrameCount = 1;
    float frameTime = 0.12f;
    sf::Clock animationClock;

public:
    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void move(float x, float y)
    {
        body.move(x, y);
    }

protected:
    virtual void updateAnimation();
    void setTexture(const sf::Texture& tex, int frames);
    virtual void onAnimationFinished() {}

};

#endif
