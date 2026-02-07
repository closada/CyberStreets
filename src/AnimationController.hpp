#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class AnimationController
{
public:
    AnimationController();

    // --- animaciones ---
    void addAnimation(
        const std::string& name,
        const sf::Texture& texture,
        int frameCount,
        float frameTime,
        bool loop
    );

    void play(const std::string& name, bool restart = false);
    void update();

    bool isFinished() const;

    // --- render ---
    void setPosition(const sf::Vector2f& pos);
    void setScale(const sf::Vector2f& scale);
    void draw(sf::RenderWindow& window);

    const std::string& getCurrentAnimation() const;

    const sf::Sprite& getSprite() const { return sprite; }



private:
    // -------------------------
    // Estructura interna
    // -------------------------
    struct AnimationData
    {
        const sf::Texture* texture;
        int frameCount;
        float frameTime;
        bool loop;
    };

    // -------------------------
    // Datos de animaciones
    // -------------------------
    std::unordered_map<std::string, AnimationData> animations;

    std::string currentAnimation;

    int currentFrame = 0;
    float elapsedTime = 0.f;
    bool finished = false;

    // reloj interno (manda el reloj)
    sf::Clock clock;

    // sprite visual
    sf::Sprite sprite;
};
