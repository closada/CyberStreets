#include "Menu.hpp"
#include <stdexcept>
#include <cmath>


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

    // ---------- NOMBRE DEL AVATAR ----------
    avatarNameText.setFont(font);
    avatarNameText.setCharacterSize(16);
    avatarNameText.setFillColor(sf::Color::Yellow);
    avatarNameText.setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f + 70.f
    );

    sf::FloatRect nb = avatarNameText.getLocalBounds();
    avatarNameText.setOrigin(nb.width / 2.f, nb.height / 2.f);


    // ---------- AVATARES ----------
    for (int i = 0; i < 3; ++i)
    {
        avatars[i].setSize({70.f, 70.f});
        avatars[i].setOrigin(35.f, 35.f);

        avatars[i].setPosition(
            windowSize.x / 2.f + (i - 1) * 120.f,
            windowSize.y / 2.f
        );
    }

    avatars[0].setFillColor(sf::Color::Green);
    avatars[1].setFillColor(sf::Color::Blue);
    avatars[2].setFillColor(sf::Color::Red);



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
            window.draw(titleText);

    float t = animClock.getElapsedTime().asSeconds();

    for (int i = 0; i < 3; ++i)
    {
        if (i == avatarIndex)
        {
            float scale = 1.15f + std::sin(t * 4.f) * 0.05f;
            avatars[i].setScale(scale, scale);

            avatars[i].setOutlineThickness(4.f);
            avatars[i].setOutlineColor(sf::Color::White);
            avatars[i].setFillColor(
                sf::Color(
                    avatars[i].getFillColor().r,
                    avatars[i].getFillColor().g,
                    avatars[i].getFillColor().b,
                    255
                )
            );
        }
        else
        {
            avatars[i].setScale(0.9f, 0.9f);
            avatars[i].setOutlineThickness(0.f);
            avatars[i].setFillColor(
                sf::Color(
                    avatars[i].getFillColor().r,
                    avatars[i].getFillColor().g,
                    avatars[i].getFillColor().b,
                    120
                )
            );
        }

        // texto por cada avatar
        switch (avatarIndex)
        {
            case 0: avatarNameText.setString("AGENTE VERDE"); break;
            case 1: avatarNameText.setString("AGENTE AZUL");  break;
            case 2: avatarNameText.setString("AGENTE ROJO");  break;
        }

        // re-centrar porque cambia el ancho
        sf::FloatRect nb = avatarNameText.getLocalBounds();
        avatarNameText.setOrigin(nb.width / 2.f, nb.height / 2.f);


        window.draw(avatarNameText);

        window.draw(avatars[i]);
    }

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
        {
            moveUp();
            sound->play(SoundID::MENU_MOVE);
        }

        else if (event.key.code == sf::Keyboard::Down)
        {
            moveDown();
            sound->play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (selectedIndex == 0) // JUGAR
            {
                state = MenuState::AVATAR_SELECT;
                sound->play(SoundID::MENU_CONFIRM);
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
        {
            avatarIndex = (avatarIndex + 2) % 3;
            sound->play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Right)
        {
            avatarIndex = (avatarIndex + 1) % 3;
            sound->play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Escape)
        {
            state = MenuState::MAIN;
            sound->play(SoundID::MENU_MOVE);
        }


        else if (event.key.code == sf::Keyboard::Enter)
        {
            selectedAvatar = static_cast<AvatarType>(avatarIndex);
            startGameRequested = true;
            sound->play(SoundID::MENU_CONFIRM);
        }
    }
}

void Menu::setSoundManager(SoundManager* s)
{
    sound = s;
}
