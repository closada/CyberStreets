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



    // -------- PAUSE OVERLAY --------
    pauseTitleText.setFont(font);
    pauseTitleText.setString("PAUSA");
    pauseTitleText.setCharacterSize(48);
    pauseTitleText.setFillColor(sf::Color::White);
    pauseTitleText.setStyle(sf::Text::Bold);
    sf::FloatRect boundspause = pauseTitleText.getLocalBounds();
    pauseTitleText.setOrigin(boundspause.left + boundspause.width/2.f, boundspause.top + boundspause.height/2.f);
    pauseTitleText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 60.f);

    pauseScoreText.setFont(font);
    pauseScoreText.setCharacterSize(24);
    pauseScoreText.setFillColor(sf::Color::Yellow);
    pauseScoreText.setStyle(sf::Text::Bold);
    pauseScoreText.setOrigin(0.f,0.f); // se ajustará dinámicamente
    pauseScoreText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

    pauseMenuHintText.setFont(font);
    pauseMenuHintText.setCharacterSize(18);
    pauseMenuHintText.setFillColor(sf::Color(180,180,180));
    pauseMenuHintText.setString("M - MENU");
    sf::FloatRect hintBounds = pauseMenuHintText.getLocalBounds();
    pauseMenuHintText.setOrigin(hintBounds.left + hintBounds.width/2.f, hintBounds.top + hintBounds.height/2.f);
    pauseMenuHintText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 60.f);


    // --- LEVEL SCORE ---
    levelScoreText.setFont(font);
    levelScoreText.setCharacterSize(24);
    levelScoreText.setFillColor(sf::Color::White);
    levelScoreText.setStyle(sf::Text::Bold);

    // centramos horizontalmente, posición vertical relativa al texto de GAME OVER / NIVEL COMPLETADO
    levelScoreText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 60.f);


    // --- TEXTO PRESIONE ENTER ---
    continueHintText.setFont(font);
    continueHintText.setCharacterSize(18);
    continueHintText.setFillColor(sf::Color(200, 200, 200));
    continueHintText.setString("Presione ENTER para continuar");

    // centramos horizontalmente, abajo de los textos principales
    sf::FloatRect hintBounds2 = continueHintText.getLocalBounds();
    continueHintText.setOrigin(hintBounds2.left + hintBounds2.width / 2.f, hintBounds2.top + hintBounds2.height / 2.f);

    // posición vertical un poco debajo del texto de game over / nivel completado
    continueHintText.setPosition(400.f, 400.f); // lo ajustamos dinámicamente si querés

    showContinueHint(false);

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

    window.draw(continueHintText);


    // dibujamos el score del nivel si el texto está visible
    if(gameOverText.getFillColor() != sf::Color::Transparent || levelCompleteText.getFillColor() != sf::Color::Transparent)
        window.draw(levelScoreText);

    if (showAllCompleted)
    {
        window.draw(allCompletedText);
        window.draw(levelScoreText);
    }


}


void HUD::showGameOver(bool show, int levelScore)
{
    if (show)
    {
        gameOverText.setFillColor(sf::Color::Red);
        levelScoreText.setString("Score: " + std::to_string(levelScore) + " m");

        // re-centrar horizontal
        sf::FloatRect bounds = levelScoreText.getLocalBounds();
        levelScoreText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
    else
        gameOverText.setFillColor(sf::Color::Transparent);
}

void HUD::showLevelComplete(bool show, int levelScore)
{
    if (show)
    {
        levelCompleteText.setFillColor(sf::Color::Yellow);
        levelScoreText.setString("Score: " + std::to_string(levelScore) + " m");

        sf::FloatRect bounds = levelScoreText.getLocalBounds();
        levelScoreText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
    else
        levelCompleteText.setFillColor(sf::Color::Transparent);
}


void HUD::showAllLevelsCompleted(bool value, int socre)
{
    showAllCompleted = value;

    levelScoreText.setString("Score: " + std::to_string(socre) + " m");

    // re-centrar horizontal
    sf::FloatRect bounds = levelScoreText.getLocalBounds();
    levelScoreText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void HUD::drawPauseOverlay(float meters, sf::RenderWindow& window)
{
    int metersShow = static_cast<int>(meters / 10.f); // ajustá escala
    pauseScoreText.setString("Score: " + std::to_string(static_cast<int>(metersShow)) + " m");
    sf::FloatRect scoreBounds = pauseScoreText.getLocalBounds();
    pauseScoreText.setOrigin(scoreBounds.left + scoreBounds.width/2.f, scoreBounds.top + scoreBounds.height/2.f);

    window.draw(pauseTitleText);
    window.draw(pauseScoreText);
    window.draw(pauseMenuHintText);
}

void HUD::showContinueHint(bool show)
{
    if(show)
        continueHintText.setFillColor(sf::Color::White);
    else
        continueHintText.setFillColor(sf::Color::Transparent);
}

