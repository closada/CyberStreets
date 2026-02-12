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


    menu = std::make_unique<Menu>(window.getSize(), sound, font, gameConfig.name);


    // ---- Cargar sonidos ----
    sound.load(SoundID::MENU_MOVE, gameConfig.sounds["menu_move"]);
    sound.load(SoundID::MENU_CONFIRM, gameConfig.sounds["menu_confirm"]);
    sound.load(SoundID::HIT, gameConfig.sounds["hit"]);
    sound.load(SoundID::PLAYER_HIT, gameConfig.sounds["player_hit"]);


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


    // INICIAMOS MUSICA EN LOOP!!
    if (musicManager.load(gameConfig.music))
    {
        musicManager.play(true); // loop infinito
    }

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
            if(!isPaused)
            {
                updatePlaying(dt);
                ActualLevelScore =  static_cast<int>(levelManager->getPlayerMaxDistance() / 10.f);
            }
            break;

        case GameState::GAME_OVER:
            updateGameOver();
            break;

        case GameState::LEVEL_COMPLETED:
            updateLevelCompleted();
            break;

        case GameState::ALL_LEVELS_COMPLETED:
            break;
    }

    // Actualizamos HUD siempre para mostrar metros aunque esté pausado
    if(gameState == GameState::PLAYING)
    {
        hud.update(
            levelManager->getPlayerHealth(),
            levelManager->getPlayerMaxDistance()
        );
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

            if(isPaused)
            {
                float meters = levelManager->getPlayerMaxDistance();
                hud.drawPauseOverlay(meters, window);
            }

            break;
        }
        case GameState::GAME_OVER:
        case GameState::LEVEL_COMPLETED:
        case GameState::ALL_LEVELS_COMPLETED:
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

    if (gameState == GameState::PLAYING)
    {
         // Siempre revisamos pausa
        static bool pPrev = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            if (!pPrev) // edge detection
                isPaused = !isPaused;
            pPrev = true;
        }
        else
        {
            pPrev = false;
        }

        if (isPaused)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                gameState = GameState::MENU;
                isPaused = false;
            }
            return; // NO actualizamos ni manejamos inputs del player
        }
    }

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
        hud.showGameOver(true, ActualLevelScore);


    }

    if (levelManager->isLevelCompleted())
    {
        if (gameConfig.lastLevelCompleted < static_cast<int> (gameConfig.levels.size()) - 1)
        {
            gameConfig.lastLevelCompleted++;
            gameConfig.score = gameConfig.score + ActualLevelScore;
            SaveManager::saveProgress(pathConfig, gameConfig);
            gameState = GameState::LEVEL_COMPLETED;
            hud.showLevelComplete(true, ActualLevelScore);
        }
        else
        {
            // era el último nivel
            gameConfig.lastLevelCompleted++;
            gameConfig.score = gameConfig.score + ActualLevelScore;
            SaveManager::saveProgress(pathConfig, gameConfig);
            gameState = GameState::ALL_LEVELS_COMPLETED;
            hud.showAllLevelsCompleted(true, gameConfig.score);
        }
    }


    hud.update(
        levelManager->getPlayerHealth(),
        levelManager->getPlayerMaxDistance()
    );

    camera = levelManager->getCamera();
}


void Game::updateGameOver()
{
    // Mostramos el mensaje de "Presione ENTER"
    hud.showContinueHint(true);

    // Detectar ENTER
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        hud.showGameOver(false, 0); // ocultamos game over
        hud.showContinueHint(false);

        // Reiniciamos el nivel actual
        startGame(); // startGame carga el nivel según gameConfig.lastLevelCompleted
    }
}



void Game::updateLevelCompleted()
{
    hud.showContinueHint(true);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        hud.showLevelComplete(false, 0); // ocultamos "nivel completado"
        hud.showContinueHint(false);

        // Arrancamos siguiente nivel
        startGame();
    }
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
    int nextLevelIndex = gameConfig.lastLevelCompleted;

    // Si no hay más niveles
    if (nextLevelIndex >= static_cast<int> (gameConfig.levels.size()))
    {
        gameState = GameState::ALL_LEVELS_COMPLETED;
        hud.showAllLevelsCompleted(true, gameConfig.score);
        return;
    }

    std::string levelFile = gameConfig.levels[nextLevelIndex].file;

    levelManager = std::make_unique<LevelManager>(
        levelFile,
        textures,
        sound,
        *enemyFactory,
        *avatarFactory,
        selectedAvatarId
    );

    camera = levelManager->getCamera();
    gameState = GameState::PLAYING;
}



