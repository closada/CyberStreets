#include <iostream>
#include <algorithm>

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
        enemy.updateAI(player.getPosition());

    // --- SEPARACIÓN ENTRE ENEMIGOS ---
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            sf::FloatRect a = enemies[i].getBounds();
            sf::FloatRect b = enemies[j].getBounds();

            if (a.intersects(b))
            {
                float overlapY = (a.top + a.height) - b.top;

                if (overlapY > 0.f)
                {
                    enemies[i].move(0.f, -1.f);
                    enemies[j].move(0.f, 1.f);
                }
            }
        }
    }


    // --- ATAQUE DEL PLAYER ---
    if (player.isAttacking())
    {
        for (auto& enemy : enemies)
        {
            if (player.getAttackBounds().intersects(enemy.getBounds()))
            {
                enemy.takeHit(player.getPosition());

            }
        }
    }



    // --- DAÑO AL PLAYER ---
    bool collidingWithEnemy = false;

    // --- ATAQUE DEL ENEMIGO ---
    for (auto& enemy : enemies)
    {
        if (enemy.isAttacking() &&
            enemy.getAttackBounds().intersects(player.getBounds()))
        {
            player.takeDamage(10);
            collidingWithEnemy = true;
        }
    }

    // --- FEEDBACK VISUAL (solo visual) ---
    if (collidingWithEnemy)
        player.setColor(sf::Color::Yellow);
    else
        player.setColor(sf::Color::Green);


    /* para ver por consola la vida del player */
    static int lastHealth = -1;

    if (player.getHealth() != lastHealth)
    {
        lastHealth = player.getHealth();
        std::cout << "Player Health: " << lastHealth << std::endl;
    }

    /* para ver por consola la vida de los enemigos */
    for (auto& enemy : enemies)
        std::cout << "Enemigo Health: " << enemy.getHealth() << std::endl;


    // limpieza de enemigos muertos
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e)
            {
                return e.isDead();
            }),
        enemies.end()
    );

}




void Game::render()
{
    window.clear(sf::Color::Black);

    player.draw(window);
    player.drawAttack(window);




    for (auto& enemy : enemies)
    {
        enemy.draw(window);
        enemy.drawAttack(window);

    }
    window.display();
}


