#include "Menu.hpp"
#include <stdexcept>

#include <iostream>

Menu::Menu(const sf::Vector2u& windowSize)
: selectedIndex(0)
{
    startGameRequested = false;
    state = MenuState::MAIN;
    avatarIndex = 0;

    if (!font.loadFromFile("assets/PressStart2P-Regular.ttf"))
        throw std::runtime_error("No se pudo cargar la fuente del menú");


    // ---------- TITULO AVATAR ----------
    titleText.setFont(font);
    titleText.setString("ELIGE TU AVATAR");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);

    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    titleText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 120.f);

    // ---------- AYUDA ----------
    hintText.setFont(font);
    hintText.setString("ENTER confirmar - ESC volver");
    hintText.setCharacterSize(16);
    hintText.setFillColor(sf::Color(180, 180, 180));

    sf::FloatRect hb = hintText.getLocalBounds();
    hintText.setOrigin(hb.width / 2.f, hb.height / 2.f);
    hintText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 120.f);


    // DE MENU PRINCIPAL
    sf::Text play("JUGAR", font, 32);
    sf::Text exit("SALIR", font, 32);

    play.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 40.f);
    exit.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 10.f);

    // centrar textos
    for (auto* text : {&play, &exit})
    {
        sf::FloatRect bounds = text->getLocalBounds();
        text->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
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
    if (state == MenuState::MAIN)
    {
        for (const auto& option : options)
            window.draw(option);
    }
    else if (state == MenuState::AVATAR_SELECT)
    {
        sf::RectangleShape avatar;
        avatar.setSize({80.f, 80.f});
        avatar.setOrigin(40.f, 40.f);
        avatar.setPosition(
            window.getSize().x / 2.f,
            window.getSize().y / 2.f
        );

        if (avatarIndex == 0) avatar.setFillColor(sf::Color::Green);
        if (avatarIndex == 1) avatar.setFillColor(sf::Color::Blue);
        if (avatarIndex == 2) avatar.setFillColor(sf::Color::Red);

        window.draw(titleText);
        window.draw(avatar);
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

AvatarType Menu::getSelectedAvatar() const
{
    return selectedAvatar;
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
            moveUp();

        else if (event.key.code == sf::Keyboard::Down)
            moveDown();

        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (selectedIndex == 0) // JUGAR
            {
                state = MenuState::AVATAR_SELECT;
            }
            else // SALIR
            {
                std::exit(0);
            }
        }
    }

    // -------- AVATAR SELECT --------
    else if (state == MenuState::AVATAR_SELECT)
    {
        if (event.key.code == sf::Keyboard::Left)
            avatarIndex = (avatarIndex + 2) % 3;

        else if (event.key.code == sf::Keyboard::Right)
            avatarIndex = (avatarIndex + 1) % 3;

        else if (event.key.code == sf::Keyboard::Escape)
            state = MenuState::MAIN;

        else if (event.key.code == sf::Keyboard::Enter)
        {
            selectedAvatar = static_cast<AvatarType>(avatarIndex);
            startGameRequested = true;
        }
    }
}
