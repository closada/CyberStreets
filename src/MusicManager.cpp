#include "MusicManager.hpp"
#include <iostream>

bool MusicManager::load(const std::string& path)
{
    if (!music.openFromFile(path))
    {
        std::cerr << "[MusicManager] No se pudo cargar la musica: "
                  << path << std::endl;
        return false;
    }

    return true;
}

void MusicManager::play(bool loop)
{
    music.setLoop(loop);
    music.play();
}

void MusicManager::stop()
{
    music.stop();
}

void MusicManager::setVolume(float volume)
{
    music.setVolume(volume);
}

