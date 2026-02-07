#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "HUD.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"
#include "InputController.hpp"
#include "LevelGoal.hpp"
#include "ResourceAliases.hpp"

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
    std::unique_ptr<Player> player;
    //std::vector<Enemy> enemies;
    InputController input;

    AvatarType selectedAvatar = AvatarType::GREEN;

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
    void spawnInitialEnemies();

    // SFX
    SoundManager sound;
    SoundBufferManager sounds;

    // CAMARA QUE SIGUE AL PLAYER
    sf::View camera;
    float levelLeftLimit;
    float levelRightEnd;

    // TEXTURAS
    TextureManager textures;

    // LEVEL GOAL
    std::unique_ptr<LevelGoal> levelGoal;
    sf::Texture goalTex;


public:
    Game();
    void run();
};


#endif
