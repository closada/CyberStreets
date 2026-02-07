#include "Entity.hpp"

// -------------------------------------------------
// Constructor
// -------------------------------------------------
Entity::Entity(const sf::Vector2f& startPos)
{
    body.setPosition(startPos);
}

// -------------------------------------------------
// Draw
// -------------------------------------------------
void Entity::draw(sf::RenderWindow& window)
{
    animation.setPosition(body.getPosition());
    animation.draw(window);
}

// -------------------------------------------------
// Transform
// -------------------------------------------------
void Entity::setPosition(const sf::Vector2f& pos)
{
    body.setPosition(pos);
}

sf::Vector2f Entity::getPosition() const
{
    return body.getPosition();
}

// -------------------------------------------------
// Bounds
// -------------------------------------------------
sf::FloatRect Entity::getBounds() const
{
    return body.getGlobalBounds();
}

// -------------------------------------------------
// Animation access
// -------------------------------------------------
AnimationController& Entity::getAnimation()
{
    return animation;
}

// -------------------------------------------------
// get animation str
// -------------------------------------------------
const std::string& Entity::getCurrentAnimation() const
{
    return animation.getCurrentAnimation();
}
