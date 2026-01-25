#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <map>
#include <string>

enum class SoundID
{
    MENU_MOVE,
    MENU_CONFIRM,
    HIT,
    PLAYER_HIT
};

class SoundManager
{
private:
    std::map<SoundID, sf::SoundBuffer> buffers;
    sf::Sound sound;

public:
    SoundManager();

    void play(SoundID id);
};

#endif

