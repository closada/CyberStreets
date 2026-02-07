#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationController.hpp"

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    void draw(sf::RenderWindow& window);

    // --- transform ---
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

    sf::FloatRect getBounds() const;

    const std::string& getCurrentAnimation() const;


protected:
    Entity(const sf::Vector2f& startPos);

    AnimationController& getAnimation();

    sf::RectangleShape body;        // colisión / lógica
    AnimationController animation;  // visual
};
