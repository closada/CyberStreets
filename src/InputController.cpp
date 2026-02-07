#include "InputController.hpp"
#include <SFML/Window/Keyboard.hpp>

InputController::InputController()
{
}

// -------------------------------------------------
// Lee teclado y genera intención del jugador
// -------------------------------------------------
InputCommand InputController::pollInput() const
{
    InputCommand command;

    // --- movimiento ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        command.movement.x -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        command.movement.x += 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        command.movement.y -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        command.movement.y += 1.f;

    // --- ataque ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        command.attackPressed = true;

    return command;
}

