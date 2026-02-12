#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>

class MusicManager
{
private:
    sf::Music music;

public:
    bool load(const std::string& path);
    void play(bool loop = true);
    void stop();
    void setVolume(float volume);
};

#endif

