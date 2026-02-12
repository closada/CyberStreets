#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "HUD.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"
#include "InputController.hpp"
#include "LevelGoal.hpp"
#include "ResourceAliases.hpp"
#include "LevelManager.hpp"
#include "GameConfigLoader.hpp"
#include "SaveManager.hpp"
#include "EnemyConfigLoader.hpp"
#include "AvatarConfigLoader.hpp"
#include "EnemyFactory.hpp"
#include "AvatarFactory.hpp"


#include <memory>
#include <vector>

enum class GameState
{
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED,
    ALL_LEVELS_COMPLETED
};

class Game
{
private:
    GameState gameState;

    sf::RenderWindow window;

    // PARA FONT
    sf::Font font;

    HUD hud;
    std::unique_ptr<Menu> menu;
    InputController input;
    GameConfig gameConfig;

    std::string pathConfig;

    std::unique_ptr<LevelManager> levelManager;


    std::string selectedAvatarId;

    // para manejo de sfx en player
    bool wasPlayerAttacking = false;


        // LOADERS!
    EnemyConfigLoader enemyConfigLoader;
    AvatarConfigLoader avatarConfigLoader;

    std::unique_ptr<EnemyFactory> enemyFactory;
    std::unique_ptr<AvatarFactory> avatarFactory;



    // control dt motor pc
    sf::Clock deltaClock;
    float dt;

    void processEvents();
    void update();
    void render();
    void handleInput();

    void updateMenu();
    void updatePlaying(float dt);
    void updateGameOver();
    void updateLevelCompleted();
    void startGame();

    // SFX
    SoundManager sound;
    SoundBufferManager sounds;

    // CAMARA QUE SIGUE AL PLAYER
    sf::View camera;

    // TEXTURAS
    ResourceManager<sf::Texture, std::string> textures;



public:
    Game();
    void run();
};


#endif
