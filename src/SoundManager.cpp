#include "SoundManager.hpp"
#include <iostream>

void SoundManager::load(SoundID id, const std::string& path)
{
    buffers.load(id, path);
}

void SoundManager::play(SoundID id)
{
    try
    {
        sound.setBuffer(buffers.get(id));
        sound.play();
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "[SoundManager] Sonido no cargado: "
                  << static_cast<int>(id) << std::endl;
        // NO se cae el juego
    }
}
