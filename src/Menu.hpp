#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Avatar.hpp"
#include "SoundManager.hpp"
#include <array>

enum class MenuState
{
    MAIN,
    AVATAR_SELECT
};

struct AvatarEntry
{
    std::string id;        // "biker"
    std::string name;      // "Biker"
    sf::Texture* texture;
    sf::Sprite sprite;
};

class Menu
{
private:
    MenuState state;
    sf::Font& font;
    std::vector<sf::Text> options;
    int selectedIndex;

    //AvatarType selectedAvatar;

    bool startGameRequested;

    // textos
    sf::Text titleText;
    sf::Text hintText;
    sf::Text avatarNameText;

    // flechas
    sf::Text leftArrow;
    sf::Text rightArrow;

    // animacion
    sf::Clock animClock;

    //avatares
    std::vector<AvatarEntry> avatars;
    int avatarIndex = 0;
    std::string selectedAvatarId;



    // sfx
    SoundManager& sound;



public:
    Menu(const sf::Vector2u& windowSize, SoundManager& sound, sf::Font& font);

    void moveUp();
    void moveDown();

    int getSelectedIndex() const;

    void update();
    void draw(sf::RenderWindow& window);

    bool wantsToStartGame() const;
    const std::string& getSelectedAvatarId() const;
    void reset();

    void handleEvent(const sf::Event& event);

    bool wantsToExit() const;
    bool exit;

    void setAvatars(const std::vector<std::pair<std::string, std::string>>& avatarData,
                      const std::unordered_map<std::string, sf::Texture*>& textures,
                      const sf::Vector2u& windowSize);

};


#endif

