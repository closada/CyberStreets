#include "Game.hpp"

Game::Game()
: window(sf::VideoMode(800, 600), "Cyber Streets"),
  player(400.f, 300.f)
{
    window.setFramerateLimit(60);

    enemies.emplace_back(800.f, 300.f);
    enemies.emplace_back(900.f, 200.f);
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update()
{
    player.update();
    player.keepInside(window.getSize());

    for (auto& enemy : enemies)
        enemy.update();

    /* si colisionan enemigos con player, tiene daño */
    bool isColliding = false;

    for (auto& enemy : enemies)
    {
        if (player.getBounds().intersects(enemy.getBounds()))
        {
            isColliding = true;
            break;
        }
    }

    if (isColliding)
        player.setColor(sf::Color::Yellow);
    else
        player.setColor(sf::Color::Green);

    /* si el juegador esta atacando, analizamos si golpea a un enemigo */
    if (player.isAttacking())
    {
        for (auto& enemy : enemies)
        {
            if (player.getAttackBounds().intersects(enemy.getBounds()))
            {
                // reacción simple: empujamos al enemigo
                enemy.takeHit();
            }
        }
    }


}




void Game::render()
{
    window.clear(sf::Color::Black);

    player.draw(window);
    player.drawAttack(window);

    for (auto& enemy : enemies)
        enemy.draw(window);

    window.display();
}


