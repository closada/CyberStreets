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


    // avatares para menu
    textures.load(TextureID::BikerAvatar,      "assets/sprites/biker_avatar.png");
    textures.load(TextureID::CyborgAvatar,      "assets/sprites/cyborg_avatar.png");
    textures.load(TextureID::PunkAvatar,      "assets/sprites/punk_avatar.png");


    // pasamos los sprites al menu para usarlos
    menu.setAvatarTextures({
        &textures.get(TextureID::BikerAvatar),
        &textures.get(TextureID::CyborgAvatar),
        &textures.get(TextureID::PunkAvatar)
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
            levelManager->draw(window);

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
    if (gameState != GameState::PLAYING || !levelManager)
        return;

    InputCommand cmd = input.pollInput();
    levelManager->handleInput(cmd, dt);
}



void Game::updatePlaying(float dt)
{
    levelManager->update(dt);

    if (levelManager->isPlayerDead())
    {
        gameState = GameState::GAME_OVER;
        hud.showGameOver(true);
    }

    if (levelManager->isLevelCompleted())
    {
        gameState = GameState::LEVEL_COMPLETED;
        hud.showLevelComplete(true);
    }

    hud.update(
        levelManager->getPlayerHealth(),
        levelManager->getPlayerMaxDistance()
    );

    camera = levelManager->getCamera();
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
    levelManager = std::make_unique<LevelManager>(
        "assets/config/levels/level1.json",
        textures,
        sound,
        selectedAvatar
    );

    camera = levelManager->getCamera();
    gameState = GameState::PLAYING;
}


