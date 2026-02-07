#pragma once
#include <SFML/Graphics.hpp>

struct Animation
{
    sf::Texture* texture = nullptr;
    int frameCount = 0;
    float frameTime = 0.f;
    bool loop = true;
};

