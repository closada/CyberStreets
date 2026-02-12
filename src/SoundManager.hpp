#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <vector>
#include "ResourceManager.hpp"
#include "ResourceAliases.hpp"



class SoundManager
{
private:
    SoundBufferManager buffers;
    std::vector<sf::Sound> sounds;

    void removeStoppedSounds();

public:
    void load(SoundID id, const std::string& path);
    void play(SoundID id);
};

#endif
