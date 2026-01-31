#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "HUD.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"

#include <vector>




enum class GameState
{
    MENU,
    PLAYING,
    GAME_OVER
};

class Game
{
private:
    GameState gameState;

    sf::RenderWindow window;
    HUD hud;
    Menu menu;
    Player player;
    std::vector<Enemy> enemies;
    AvatarType selectedAvatar = AvatarType::GREEN;

    void processEvents();
    void update();
    void render();
    void handleInput();

    void updateMenu();
    void updatePlaying();
    void updateGameOver();
    void startGame();
    void spawnInitialEnemies();


    // SFX
    SoundManager sound;

    // CAMARA QUE SIGUE AL PLAYER
    sf::View camera;
    float levelLeftLimit;
    float levelRightEnd;

public:
    Game();
    void run();
};


#endif

