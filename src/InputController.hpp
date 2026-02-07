#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct InputCommand
{
    sf::Vector2f movement;   // (-1, 0), (0, 1), etc
    bool attackPressed = false;
};

class InputController
{
public:
    InputController();

    // Lee teclado y genera intención
    InputCommand pollInput() const;
};

