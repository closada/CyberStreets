#include <iostream>
#include <algorithm>

#include "Game.hpp"


Game::Game()
: window(sf::VideoMode(800, 600), "Cyber Streets"),
  hud(window.getSize(), 100),
  menu(window.getSize())
{


    gameState = GameState::MENU;

    window.setFramerateLimit(60);


    // definiciones de camara
    camera = window.getDefaultView();
    levelLeftLimit = camera.getCenter().x - camera.getSize().x / 2.f;

    // donde termina el nivel
    levelRightEnd = 4000.f;


    menu.setSoundManager(&sound);

    // definiciones de player
    playerIdleTex.loadFromFile("assets/sprites/player/Biker_idle.png");
    playerRunTex.loadFromFile("assets/sprites/player/Biker_run.png");
    playerAttackTex.loadFromFile("assets/sprites/player/Biker_attack.png");
    playerRunAttackTex.loadFromFile("assets/sprites/player/Biker_run_attack.png");

    player = std::make_unique<Player>(
        sf::Vector2f(400.f, 300.f),
        playerIdleTex,
        playerRunTex,
        playerAttackTex,
        playerRunAttackTex
    );
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        handleInput();
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

        if (gameState == GameState::MENU)
            menu.handleEvent(event);
    }
}

void Game::update()
{
    switch (gameState)
    {
        case GameState::MENU:
            updateMenu();
            break;

        case GameState::PLAYING:
            updatePlaying();
            break;

        case GameState::GAME_OVER:
            updateGameOver();
            break;
    }
}





void Game::render()
{
    window.clear(sf::Color::Black);

    if (gameState == GameState::MENU)
    {
        window.setView(window.getDefaultView());
        menu.draw(window);
    }
    else if (gameState == GameState::PLAYING)
    {
        // ---- MUNDO ----
        window.setView(camera);

        if (player)
        {
            player->draw(window);
            player->drawAttackBox(window);
        }



        /*for (auto& enemy : enemies)
        {
            enemy.draw(window);
            enemy.drawAttack(window);
        }*/

        // ---- HUD ----
        window.setView(window.getDefaultView());
        hud.draw(window);
    }
    else if (gameState == GameState::GAME_OVER)
    {
        window.setView(window.getDefaultView());
        hud.draw(window);
    }

    window.display();
}




void Game::handleInput()
{
    if (gameState != GameState::PLAYING || !player)
        return;

    InputCommand cmd = input.pollInput();

    player->handleMovement(cmd.movement);

    if (cmd.attackPressed)
        player->handleAttack();
}


void Game::updatePlaying()
{
    player->update(0.f);
    player->keepInside(camera);
    hud.update(player->getHealth());


    // CAMARA
    sf::Vector2f camPos = camera.getCenter();

    // dead zone horizontal
    float deadZone = 120.f;

    float dx = player->getPosition().x - camPos.x;

    if (dx > deadZone)
    {
        camPos.x += dx - deadZone;
    }

    camera.setCenter(camPos);
    //window.setView(camera);


    /*// enemigos
    for (auto& enemy : enemies)
    {
        enemy.update();
        enemy.updateAI(player.getPosition());
    }

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

                // empuje vertical suave (estilo arcade)
                if (overlapY > 0.f)
                {
                    enemies[i].move({0.f, -0.5f});
                    enemies[j].move({0.f,  0.5f});
                }
            }
        }
    }


    // ataque player enemy
    if (player.canHit())
    {
        for (auto& enemy : enemies)
        {
            if (player.getAttackBounds().intersects(enemy.getBounds()))
            {
                enemy.takeHit(player.getPosition());
                player.markHit();
                sound.play(SoundID::HIT);
                break;
            }
        }
    }


    // ataque enemy player
    for (auto& enemy : enemies)
    {
        if (enemy.isAttacking() &&
            enemy.getAttackBounds().intersects(player.getBounds()))
        {
            if (player.canReceiveDamage())
            {
                player.takeDamage(10);
                sound.play(SoundID::PLAYER_HIT);
            }
        }
    }

    // eliminar enemigos muertos
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.isDead(); }),
        enemies.end()
    );

    if (player.isDead())
    {
        gameState = GameState::GAME_OVER;
        hud.showGameOver(true);
    }*/
}

void Game::updateGameOver()
{
    // por ahora no hacemos nada
    // más adelante: ENTER para volver al menú
}

void Game::updateMenu()
{
    menu.update();

    if (menu.wantsToStartGame())
    {
        selectedAvatar = menu.getSelectedAvatar();
        startGame();
        menu.reset();
    }
}




void Game::startGame()
{
    /*player.setAvatar(selectedAvatar);

    enemies.clear();
    spawnInitialEnemies();*/

    gameState = GameState::PLAYING;
}

void Game::spawnInitialEnemies()
{
    /*enemies.clear();
    enemies.emplace_back(500.f, 300.f);
    enemies.emplace_back(650.f, 300.f);*/
}

