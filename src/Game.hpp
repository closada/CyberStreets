#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"

#include <vector>


class Game
{
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Enemy> enemies;

    void processEvents();
    void update();
    void render();

public:
    Game();
    void run();
};


#endif

