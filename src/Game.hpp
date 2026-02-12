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

#include <memory>
#include <vector>

enum class GameState
{
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED
};

class Game
{
private:
    GameState gameState;

    sf::RenderWindow window;
    HUD hud;
    Menu menu;
    InputController input;
    GameConfig gameConfig;

    std::string pathConfig;

    std::unique_ptr<LevelManager> levelManager;


    AvatarType selectedAvatar = AvatarType::Biker;

    // para manejo de sfx en player
    bool wasPlayerAttacking = false;


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
