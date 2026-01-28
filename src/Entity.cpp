#include "Entity.hpp"

void Entity::draw(sf::RenderWindow& window)
{
    sprite.setPosition(body.getPosition());
    window.draw(sprite);
}

sf::FloatRect Entity::getBounds() const
{
    return body.getGlobalBounds();
}

sf::Vector2f Entity::getPosition() const
{
    return body.getPosition();
}

void Entity::setTexture(const sf::Texture& tex, int frames)
{
    texture = tex;
    sprite.setTexture(texture);

    currentFrameCount = frames;
    frameCount = frames;
    currentFrame = 0;

    sprite.setTextureRect({
        0,
        0,
        frameWidth,
        frameHeight
    });

    animationClock.restart();
}


void Entity::updateAnimation()
{
    if (animationClock.getElapsedTime().asSeconds() >= frameTime)
    {
        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0;

        sprite.setTextureRect(sf::IntRect(
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight
        ));

        animationClock.restart();
    }
}



