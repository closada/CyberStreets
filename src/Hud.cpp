#include "HUD.hpp"
#include <stdexcept>

HUD::HUD(const sf::Vector2u& windowSize, int maxHealth)
: maxHealth(maxHealth)
{
    if (!font.loadFromFile("assets/PressStart2P-Regular.ttf"))
        throw std::runtime_error("No se pudo cargar la fuente");

    // barra fondo
    healthBarBack.setSize({200.f, 20.f});
    healthBarBack.setFillColor(sf::Color(60, 60, 60));
    healthBarBack.setPosition(20.f, 20.f);

    // barra frontal
    healthBarFront.setSize({200.f, 20.f});
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setPosition(20.f, 20.f);

    // texto game over
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);

    // centrar origen
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    // centrar en pantalla
    gameOverText.setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f
    );

    gameOverText.setFillColor(sf::Color::Transparent);
}


void HUD::update(int currentHealth)
{
    float healthPercent =
        static_cast<float>(currentHealth) / maxHealth;

    if (healthPercent < 0.f)
        healthPercent = 0.f;

    healthBarFront.setSize({
        200.f * healthPercent,
        healthBarFront.getSize().y
    });

    // color dinámico
    if (healthPercent > 0.6f)
        healthBarFront.setFillColor(sf::Color::Green);
    else if (healthPercent > 0.3f)
        healthBarFront.setFillColor(sf::Color::Yellow);
    else
        healthBarFront.setFillColor(sf::Color::Red);
}

void HUD::draw(sf::RenderWindow& window)
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);
    window.draw(gameOverText);
}

void HUD::showGameOver(bool show)
{
    if (show)
        gameOverText.setFillColor(sf::Color::Red);
    else
        gameOverText.setFillColor(sf::Color::Transparent);
}

