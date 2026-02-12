#include "LevelGoal.hpp"
#include "Player.hpp"

LevelGoal::LevelGoal(float xPosition, float groundY, const sf::Texture& tex)
: reached(false)
{
    // Agregamos animación de bandera
    animation.addAnimation(
        "idle",
        tex,
        5,          // cantidad de frames (ajustá esto)
        0.15f,      // tiempo por frame
        true        // loop
    );

    animation.play("idle");


    animation.setPosition({ xPosition, groundY });
    animation.setScale({2.f, 2.f});
}

void LevelGoal::update(const Player& player)
{
    // Animar siempre
    animation.update();

    // Actualizamos bounds desde el sprite animado
    triggerBounds = animation.getSprite().getGlobalBounds();

//    if (reached)
//        return;

    if (player.getBounds().intersects(triggerBounds))
    {
        reached = true;
    }
    else
    {
        reached = false;
    }
}

bool LevelGoal::isReached() const
{
    return reached;
}

void LevelGoal::draw(sf::RenderWindow& window)
{
    animation.draw(window);
}
