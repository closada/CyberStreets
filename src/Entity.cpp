#include "Entity.hpp"

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::FloatRect Entity::getBounds() const
{
    return body.getGlobalBounds();
}

sf::Vector2f Entity::getPosition() const
{
    return body.getPosition();
}

