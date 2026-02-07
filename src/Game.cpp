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
    levelRightEnd = 1000.f;



    sound.load(SoundID::MENU_MOVE,    "assets/sounds/menu_move.wav");
    sound.load(SoundID::MENU_CONFIRM, "assets/sounds/menu_confirm.wav");
    sound.load(SoundID::HIT,          "assets/sounds/hit.wav");
    sound.load(SoundID::PLAYER_HIT,   "assets/sounds/player_hit.wav");

    menu.setSoundManager(&sound);



    // definiciones de flag
    textures.load(TextureID::GoalFlag,         "assets/sprites/goal_flag.png");

    // Biker
    textures.load(TextureID::BikerIdle,      "assets/sprites/player/Biker_idle.png");
    textures.load(TextureID::BikerRun,       "assets/sprites/player/Biker_run.png");
    textures.load(TextureID::BikerAttack,    "assets/sprites/player/Biker_attack.png");
    textures.load(TextureID::BikerRunAttack, "assets/sprites/player/Biker_run_attack.png");

    // Cyborg
    textures.load(TextureID::CyborgIdle,      "assets/sprites/player/Cyborg_idle.png");
    textures.load(TextureID::CyborgRun,       "assets/sprites/player/Cyborg_run.png");
    textures.load(TextureID::CyborgAttack,    "assets/sprites/player/Cyborg_attack.png");
    textures.load(TextureID::CyborgRunAttack, "assets/sprites/player/Cyborg_run_attack.png");

    // Punk
    textures.load(TextureID::PunkIdle,      "assets/sprites/player/Punk_idle.png");
    textures.load(TextureID::PunkRun,       "assets/sprites/player/Punk_run.png");
    textures.load(TextureID::PunkAttack,    "assets/sprites/player/Punk_attack.png");
    textures.load(TextureID::PunkRunAttack, "assets/sprites/player/Punk_run_attack.png");


    levelGoal = std::make_unique<LevelGoal>(
        levelRightEnd,
        520.f,
        textures.get(TextureID::GoalFlag)
    );


    // pasamos los sprites al menu para usarlos
    menu.setAvatarTextures({
        &textures.get(TextureID::BikerIdle),
        &textures.get(TextureID::CyborgIdle),
        &textures.get(TextureID::PunkIdle)
    });

}

void Game::run()
{
    while (window.isOpen())
    {
        dt = deltaClock.restart().asSeconds();


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
            updatePlaying(dt);
            break;

        case GameState::GAME_OVER:
            updateGameOver();
            break;
        case GameState::LEVEL_COMPLETED:
            updateLevelCompleted();
            break;
    }
}





void Game::render()
{
    window.clear(sf::Color::Black);

    switch (gameState)
    {
        case GameState::MENU:
        {
            window.setView(window.getDefaultView());
            menu.draw(window);
            break;
        }

        case GameState::PLAYING:
        {
            // ---- MUNDO ----
            window.setView(camera);

            levelGoal->draw(window);
            player->draw(window);
            player->drawAttackBox(window);

            // ---- HUD ----
            window.setView(window.getDefaultView());
            hud.draw(window);
            break;
        }

        case GameState::GAME_OVER:
        case GameState::LEVEL_COMPLETED:
        {
            window.setView(window.getDefaultView());
            hud.draw(window);
            break;
        }
    }

    window.display();
}





void Game::handleInput()
{
    if (gameState != GameState::PLAYING || !player)
        return;

    InputCommand cmd = input.pollInput();

    player->handleMovement(cmd.movement, dt);

    if (cmd.attackPressed)
        player->handleAttack();
}


void Game::updatePlaying(float dt)
{
    player->update(dt);
    player->keepInside(camera);


    // sistema just_pressed para sfx en player
    bool isAttackingNow = player->isAttacking();

    if (isAttackingNow && !wasPlayerAttacking)
    {
        sound.play(SoundID::PLAYER_HIT);
    }

    wasPlayerAttacking = isAttackingNow;



    levelGoal->update(*player);

    if (levelGoal->isReached())
    {
        gameState = GameState::LEVEL_COMPLETED;
        hud.showLevelComplete(true);
    }

    hud.update(player->getHealth(), player->getMaxDistanceReached());


    // CAMARA
    sf::Vector2f camPos = camera.getCenter();

    // dead zone horizontal
    float deadZone = 120.f;

    float dx = player->getPosition().x - camPos.x;

    if (dx > deadZone)
    {
        camPos.x += dx - deadZone;
    }


    // --- LÍMITE DERECHO DE CÁMARA ---
    float halfView = camera.getSize().x / 2.f;
    float maxCamX = levelRightEnd + 20.f - halfView; // los 20.f es para que tenga un "respiro" visual

    if (camPos.x > maxCamX)
    {
        camPos.x = maxCamX;
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


void Game::updateLevelCompleted()
{
        // enter para volver al menu
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
        TextureID idle, run, attack, runAttack;

        switch (selectedAvatar)
        {
            case AvatarType::Biker:
                idle = TextureID::BikerIdle;
                run  = TextureID::BikerRun;
                attack = TextureID::BikerAttack;
                runAttack = TextureID::BikerRunAttack;
                break;

            case AvatarType::Cyborg:
                idle = TextureID::CyborgIdle;
                run  = TextureID::CyborgRun;
                attack = TextureID::CyborgAttack;
                runAttack = TextureID::CyborgRunAttack;
                break;

            case AvatarType::Punk:
                idle = TextureID::PunkIdle;
                run  = TextureID::PunkRun;
                attack = TextureID::PunkAttack;
                runAttack = TextureID::PunkRunAttack;
                break;
        }

        player = std::make_unique<Player>(
            sf::Vector2f(400.f, 300.f),
            textures.get(idle),
            textures.get(run),
            textures.get(attack),
            textures.get(runAttack)
        );

        wasPlayerAttacking = false;


    /*enemies.clear();
    spawnInitialEnemies();*/

    gameState = GameState::PLAYING;
}

void Game::spawnInitialEnemies()
{
    /*enemies.clear();
    enemies.emplace_back(500.f, 300.f);
    enemies.emplace_back(650.f, 300.f);*/
}

