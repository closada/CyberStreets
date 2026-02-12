#include <iostream>
#include <algorithm>

#include "Game.hpp"


Game::Game()
: gameState(GameState::MENU),
  window(sf::VideoMode(800, 600), ""),
  hud(window.getSize(), 100),
  //menu(window.getSize(), sound, font),
  enemyConfigLoader(textures),
  avatarConfigLoader(textures)
{
    window.setFramerateLimit(60);

    // ---- Cargar config ----
    pathConfig = "assets/config/config.json";
    gameConfig = GameConfigLoader::loadFromFile(pathConfig);

    window.setTitle(gameConfig.name);


    std::cout << "Typography path: " << gameConfig.typography << std::endl;

    if (!font.loadFromFile(gameConfig.typography))
        throw std::runtime_error("No se pudo cargar tipografia");


    menu = std::make_unique<Menu>(window.getSize(), sound, font);


    // ---- Cargar sonidos ----
    sound.load(SoundID::MENU_MOVE,    "assets/sounds/menu_move.wav");
    sound.load(SoundID::MENU_CONFIRM, "assets/sounds/menu_confirm.wav");
    sound.load(SoundID::HIT,          "assets/sounds/hit.wav");
    sound.load(SoundID::PLAYER_HIT,   "assets/sounds/player_hit.wav");

    textures.load("goalFlag", "assets/sprites/goal_flag.png");

    // ---- Load configs ----
    enemyConfigLoader.loadFromFile("assets/config/enemyTypes.json");
    avatarConfigLoader.loadFromFile("assets/config/Avatars.json");

    // ---- Factories ----
    enemyFactory = std::make_unique<EnemyFactory>(enemyConfigLoader, textures);
    avatarFactory = std::make_unique<AvatarFactory>(avatarConfigLoader, textures);

    // ---- Cargar avatares en menú ----
    std::vector<std::pair<std::string, std::string>> avatarMenuData;
    std::unordered_map<std::string, sf::Texture*> avatarTextures;

    for (const auto& pair : avatarConfigLoader.getAll())
    {
        const std::string& id = pair.first;
        const AvatarConfig& cfg = pair.second;

        std::string texId = id + "_avatar";
        textures.load(texId, cfg.avatarImg);

        avatarMenuData.push_back({ id, id });
        avatarTextures[id] = &textures.get(texId);
    }

    menu->setAvatars(avatarMenuData, avatarTextures, window.getSize());
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
            menu->handleEvent(event);
    }
}

void Game::update()
{
    switch (gameState)
    {
        case GameState::MENU:
            updateMenu();
            if(menu->wantsToExit())
                exit(0);
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
            menu->draw(window);
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
        if (gameConfig.lastLevelCompleted < gameConfig.levels.size() - 1)
            gameConfig.lastLevelCompleted++;

        SaveManager::saveProgress(pathConfig, gameConfig);

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
    menu->update();

    if (menu->wantsToStartGame())
    {
        selectedAvatarId = menu->getSelectedAvatarId();
        startGame();
        menu->reset();
    }
}


void Game::startGame()
{

    std::string avatarId = selectedAvatarId;


    int nextLevelIndex = gameConfig.lastLevelCompleted;
    std::string levelFile = gameConfig.levels[nextLevelIndex].file;

    levelManager = std::make_unique<LevelManager>(
        levelFile,
        textures,
        sound,
        *enemyFactory,
        *avatarFactory,
        avatarId
    );


    camera = levelManager->getCamera();
    gameState = GameState::PLAYING;
}


