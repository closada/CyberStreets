#include "Menu.hpp"
#include <stdexcept>
#include <cmath>


#include <iostream>

Menu::Menu(const sf::Vector2u& windowSize, SoundManager& sound, sf::Font& font)
: sound(sound),
  font(font),
  selectedIndex(0),
  avatarIndex(0),
  startGameRequested(false),
  exit(false)
{
    startGameRequested = false;
    state = MenuState::MAIN;
    avatarIndex = 0;
    exit = false;



    // ---------- TITULO AVATAR ----------
    titleText.setFont(font);
    titleText.setString("ELIGE TU AVATAR");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);

    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    titleText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 120.f);

    // ---------- AYUDA ----------
    hintText.setFont(font);
    hintText.setString("ENTER confirmar - ESC volver");
    hintText.setCharacterSize(16);
    hintText.setFillColor(sf::Color(180, 180, 180));

    sf::FloatRect hb = hintText.getLocalBounds();
    hintText.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
    hintText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 120.f);

    // ---------- NOMBRE DEL AVATAR ----------
    avatarNameText.setFont(font);
    avatarNameText.setCharacterSize(16);
    avatarNameText.setFillColor(sf::Color::Yellow);
    avatarNameText.setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f + 70.f
    );

    sf::FloatRect nb = avatarNameText.getLocalBounds();
    avatarNameText.setOrigin(nb.left + nb.width / 2.f, nb.top + nb.height / 2.f);


    // DE MENU PRINCIPAL
    sf::Text play("JUGAR", font, 32);
    sf::Text exit("SALIR", font, 32);

    play.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 40.f);
    exit.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 10.f);

    // centrar textos
    for (auto* text : {&play, &exit})
    {
        sf::FloatRect bounds = text->getLocalBounds();
        text->setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        text->setFillColor(sf::Color::White);
    }

    play.setFillColor(sf::Color::Yellow); // selección inicial

    options.push_back(play);
    options.push_back(exit);
}

void Menu::moveUp()
{
    if (selectedIndex > 0)
    {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown()
{
    if (selectedIndex < static_cast<int>(options.size()) - 1)
    {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

int Menu::getSelectedIndex() const
{
    return selectedIndex;
}

void Menu::draw(sf::RenderWindow& window)
{
    sf::Vector2f center = window.getView().getCenter();

    if (state == MenuState::MAIN)
    {
        options[0].setPosition(center.x, center.y - 40.f);
        options[1].setPosition(center.x, center.y + 10.f);

        for (const auto& option : options)
            window.draw(option);
    }
    else if (state == MenuState::AVATAR_SELECT)
    {
        titleText.setPosition(center.x, center.y - 120.f);
        hintText.setPosition(center.x, center.y + 120.f);
        avatarNameText.setPosition(center.x, center.y + 70.f);

        window.draw(titleText);

        float t = animClock.getElapsedTime().asSeconds();

        for (size_t i = 0; i < avatars.size(); ++i)
        {
            if (i == avatarIndex)
            {
                float scale = 1.15f + std::sin(t * 4.f) * 0.05f;
                avatars[i].sprite.setScale(scale, scale);
            }
            else
            {
                avatars[i].sprite.setScale(1.f, 1.f);
            }

            window.draw(avatars[i].sprite);
        }

        avatarNameText.setString(avatars[avatarIndex].name);
        sf::FloatRect nb = avatarNameText.getLocalBounds();
        avatarNameText.setOrigin(nb.left + nb.width / 2.f,
                                 nb.top + nb.height / 2.f);

        window.draw(avatarNameText);
        window.draw(hintText);
    }

}


void Menu::update()
{

}

bool Menu::wantsToStartGame() const
{
    return startGameRequested;
}

const std::string& Menu::getSelectedAvatarId() const
{
    return selectedAvatarId;
}


void Menu::reset()
{
    startGameRequested = false;
    state = MenuState::MAIN;
    selectedIndex = 0;
    avatarIndex = 0;
}

void Menu::handleEvent(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    // -------- MAIN MENU --------
    if (state == MenuState::MAIN)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            moveUp();
            sound.play(SoundID::MENU_MOVE);
        }

        else if (event.key.code == sf::Keyboard::Down)
        {
            moveDown();
            sound.play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (selectedIndex == 0) // JUGAR
            {
                state = MenuState::AVATAR_SELECT;
                sound.play(SoundID::MENU_CONFIRM);
            }
            else // SALIR
            {
                exit = true;
            }
        }
    }

    // -------- AVATAR SELECT --------
    else if (state == MenuState::AVATAR_SELECT)
    {

        if (avatars.empty())
        {
            std::cout << "avatars vacio." << std::endl;
            return;
        }


        if (event.key.code == sf::Keyboard::Left)
        {
            avatarIndex = (avatarIndex - 1 + avatars.size()) % avatars.size();
            sound.play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Right)
        {
            avatarIndex = (avatarIndex + 1) % avatars.size();
            sound.play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Escape)
        {
            state = MenuState::MAIN;
            sound.play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Enter)
        {
            selectedAvatarId = avatars[avatarIndex].id;
            startGameRequested = true;
            sound.play(SoundID::MENU_CONFIRM);
        }
    }
}


/*
void Menu::setAvatarTextures(const std::array<sf::Texture*, 3>& textures)
{
    avatarTextures = textures;

    for (int i = 0; i < 3; ++i)
    {
        avatars[i].setTexture(*avatarTextures[i]);

        sf::FloatRect bounds = avatars[i].getLocalBounds();
        avatars[i].setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
}
*/

void Menu::setAvatars(const std::vector<std::pair<std::string, std::string>>& avatarData,
                      const std::unordered_map<std::string, sf::Texture*>& textures,
                      const sf::Vector2u& windowSize)
{
    avatars.clear();

    int count = avatarData.size();

    for (int i = 0; i < count; ++i)
    {
        AvatarEntry entry;
        entry.id = avatarData[i].first;
        entry.name = avatarData[i].second;
        entry.texture = textures.at(entry.id);

        entry.sprite.setTexture(*entry.texture);

        sf::FloatRect bounds = entry.sprite.getLocalBounds();
        entry.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        float startX = windowSize.x / 2.f - ((count - 1) * 120.f) / 2.f;

        entry.sprite.setPosition(
            startX + i * 120.f,
            windowSize.y / 2.f
        );

        avatars.push_back(entry);
    }
}

bool Menu::wantsToExit() const
{
    return exit;
}
