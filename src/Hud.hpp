#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

class HUD
{
private:
    // fuente
    sf::Font font;

    // barra de vida
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;

    // texto game over
    sf::Text gameOverText;

    sf::Text levelCompleteText;

    sf::Text distanceText;
    sf::Text pauseHintText;


    sf::Text allCompletedText;
    bool showAllCompleted = false;

    int maxHealth;

public:
    HUD(const sf::Vector2u& windowSize, int maxHealth = 100);

    void update(int currentHealth, float maxDistance);
    void draw(sf::RenderWindow& window);

    void showGameOver(bool show);
    void showLevelComplete(bool show);
    void showAllLevelsCompleted(bool value);
};

#endif

