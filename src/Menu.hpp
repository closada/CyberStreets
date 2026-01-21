#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Avatar.hpp"

enum class MenuState
{
    MAIN,
    AVATAR_SELECT
};

class Menu
{
private:
    MenuState state;
    sf::Font font;
    std::vector<sf::Text> options;
    int selectedIndex;

    int avatarIndex;
    AvatarType selectedAvatar;

    bool startGameRequested;

    // textos
    sf::Text titleText;
    sf::Text hintText;

    // flechas
    sf::Text leftArrow;
    sf::Text rightArrow;


public:
    Menu(const sf::Vector2u& windowSize);

    void moveUp();
    void moveDown();

    int getSelectedIndex() const;

    void update();
    void draw(sf::RenderWindow& window);

    bool wantsToStartGame() const;
    AvatarType getSelectedAvatar() const;
    void reset();

    void handleEvent(const sf::Event& event);

};


#endif

