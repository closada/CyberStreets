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



    sound.load(SoundID::MENU_MOVE,    "assets/sounds/menu_move.wav");
    sound.load(SoundID::MENU_CONFIRM, "assets/sounds/menu_confirm.wav");
    sound.load(SoundID::HIT,          "assets/sounds/hit.wav");
    sound.load(SoundID::PLAYER_HIT,   "assets/sounds/player_hit.wav");

    menu.setSoundManager(&sound);



    // definiciones de flag
    textures.load("goalFlag",         "assets/sprites/goal_flag.png");


    // avatares para menu
    textures.load("bikerAvatar",      "assets/sprites/biker_avatar.png");
    textures.load("cyborgAvatar",      "assets/sprites/cyborg_avatar.png");
    textures.load("punkAvatar",      "assets/sprites/punk_avatar.png");


    // pasamos los sprites al menu para usarlos
    std::array<sf::Texture*, 3> avatarTex = {
        &textures.get("bikerAvatar"),
        &textures.get("cyborgAvatar"),
        &textures.get("punkAvatar")
    };

    menu.setAvatarTextures(avatarTex);

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

    std::string avatarId;

    switch(selectedAvatar)
    {
        case AvatarType::Biker:  avatarId = "biker"; break;
        case AvatarType::Cyborg: avatarId = "cyborg"; break;
        case AvatarType::Punk:   avatarId = "punk"; break;
    }

    levelManager = std::make_unique<LevelManager>(
        "assets/config/levels/level1.json",
        textures,
        sound,
        avatarId
    );

    camera = levelManager->getCamera();
    gameState = GameState::PLAYING;
}


