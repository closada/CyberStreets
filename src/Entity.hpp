#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity
{
protected:
    sf::RectangleShape body;
    float speed;

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

};

#endif

