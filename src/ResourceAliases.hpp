#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <SFML/Audio.hpp>

enum class SoundID
{
    MENU_MOVE,
    MENU_CONFIRM,
    HIT,
    PLAYER_HIT
};

using TextureManager = ResourceManager<sf::Texture, TextureID>;
using SoundBufferManager = ResourceManager<sf::SoundBuffer, SoundID>;
