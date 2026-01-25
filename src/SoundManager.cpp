#include "SoundManager.hpp"
#include <stdexcept>

SoundManager::SoundManager()
{
    if (!buffers[SoundID::MENU_MOVE].loadFromFile("assets/sounds/menu_move.wav"))
        throw std::runtime_error("No se pudo cargar menu_move.wav");

    if (!buffers[SoundID::MENU_CONFIRM].loadFromFile("assets/sounds/menu_confirm.wav"))
        throw std::runtime_error("No se pudo cargar menu_confirm.wav");

    if (!buffers[SoundID::HIT].loadFromFile("assets/sounds/hit.wav"))
        throw std::runtime_error("No se pudo cargar hit.wav");

    if (!buffers[SoundID::PLAYER_HIT].loadFromFile("assets/sounds/player_hit.wav"))
        throw std::runtime_error("No se pudo cargar player_hit.wav");
}

void SoundManager::play(SoundID id)
{
    sound.setBuffer(buffers[id]);
    sound.play();
}

