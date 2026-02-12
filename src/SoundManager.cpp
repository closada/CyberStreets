#include "SoundManager.hpp"
#include <iostream>
#include <algorithm>


void SoundManager::load(SoundID id, const std::string& path)
{
    std::cout << "Loading sound: " << path << std::endl;
    buffers.load(id, path);

}


void SoundManager::play(SoundID id)
{
    try
    {
        removeStoppedSounds();

        sounds.emplace_back(); // crea el sound dentro del vector
        sf::Sound& sound = sounds.back();

        sound.setBuffer(buffers.get(id));
        sound.play();
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "[SoundManager] Sonido no cargado: "
                  << static_cast<int>(id) << std::endl;
    }
}


void SoundManager::removeStoppedSounds()
{
    sounds.erase(
        std::remove_if(
            sounds.begin(),
            sounds.end(),
            [](const sf::Sound& s)
            {
                return s.getStatus() == sf::Sound::Stopped;
            }
        ),
        sounds.end()
    );
}
