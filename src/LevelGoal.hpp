#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationController.hpp"

class Player;

class LevelGoal
{
public:
    LevelGoal(float xPosition, float groundY, const sf::Texture& tex);

    void update(const Player& player);
    void draw(sf::RenderWindow& window);

    bool isReached() const;

private:
    AnimationController animation;
    sf::FloatRect triggerBounds;
    bool reached;
};

