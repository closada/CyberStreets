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


    // texto NIVEL COMPLETADO
    levelCompleteText.setFont(font);
    levelCompleteText.setString("NIVEL COMPLETADO");
    levelCompleteText.setCharacterSize(30);
    levelCompleteText.setFillColor(sf::Color::Red);
    levelCompleteText.setStyle(sf::Text::Bold);

    // centrar origen
    sf::FloatRect bounds2 = levelCompleteText.getLocalBounds();
    levelCompleteText.setOrigin(
        bounds2.left + bounds2.width / 2.f,
        bounds2.top + bounds2.height / 2.f
    );

    // centrar en pantalla
    levelCompleteText.setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f
    );

    levelCompleteText.setFillColor(sf::Color::Transparent);



    // TEXTO DISTANCIA MAXIMA
    distanceText.setFont(font);
    distanceText.setCharacterSize(16);
    distanceText.setFillColor(sf::Color::White);
    distanceText.setString("Record: 0 m");


    // centrado arriba
    sf::FloatRect dBounds = distanceText.getLocalBounds();
    distanceText.setOrigin(
        dBounds.left + dBounds.width / 2.f,
        dBounds.top
    );

    distanceText.setPosition(
        windowSize.x / 2.f,
        20.f
    );


    // TEXTO PAUSA
    pauseHintText.setFont(font);
    pauseHintText.setCharacterSize(14);
    pauseHintText.setFillColor(sf::Color(180, 180, 180));
    pauseHintText.setString("P - PAUSA");

    sf::FloatRect pBounds = pauseHintText.getLocalBounds();
    pauseHintText.setOrigin(
        pBounds.width,
        0.f
    );

    pauseHintText.setPosition(
        windowSize.x - 20.f,
        20.f
    );


    // TEXTO TODOS LOS NIVELES COMPLETADOS!
    allCompletedText.setFont(font);
    allCompletedText.setString("FELICITACIONES!\nCOMPLETASTE TODOS LOS NIVELES");
    allCompletedText.setCharacterSize(20);
    allCompletedText.setFillColor(sf::Color::Yellow);


        // centrar origen
    sf::FloatRect bounds3 = allCompletedText.getLocalBounds();
    allCompletedText.setOrigin(
        bounds3.left + bounds3.width / 2.f,
        bounds3.top + bounds3.height / 2.f
    );

    // centrar en pantalla
    allCompletedText.setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f
    );


}


void HUD::update(int currentHealth, float maxDistance)
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



    // --- DISTANCIA ---
    int meters = static_cast<int>(maxDistance / 10.f); // ajustá escala
    distanceText.setString("Record: " + std::to_string(meters) + " m");

    // re-centrar por cambio de texto
    sf::FloatRect bounds = distanceText.getLocalBounds();
    distanceText.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top
    );
}

void HUD::draw(sf::RenderWindow& window)
{
    window.draw(healthBarBack);
    window.draw(healthBarFront);

    window.draw(distanceText);
    window.draw(pauseHintText);

    window.draw(gameOverText);
    window.draw(levelCompleteText);

    if (showAllCompleted)
        window.draw(allCompletedText);

}


void HUD::showGameOver(bool show)
{
    if (show)
        gameOverText.setFillColor(sf::Color::Red);
    else
        gameOverText.setFillColor(sf::Color::Transparent);
}


void HUD::showLevelComplete(bool show)
{
    if (show)
        levelCompleteText.setFillColor(sf::Color::Yellow);
    else
        levelCompleteText.setFillColor(sf::Color::Transparent);
}

void HUD::showAllLevelsCompleted(bool value)
{
    showAllCompleted = value;
}


