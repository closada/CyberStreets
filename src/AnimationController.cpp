#include "AnimationController.hpp"

// -------------------------------------------------
// Estructura interna de animación
// -------------------------------------------------
struct AnimationData
{
    const sf::Texture* texture;
    int frameCount;
    float frameTime;
    bool loop;
};

// -------------------------------------------------
// Variables internas
// -------------------------------------------------
static sf::IntRect computeFrameRect(
    int frame,
    int frameWidth,
    int frameHeight
)
{
    return sf::IntRect(
        frame * frameWidth,
        0,
        frameWidth,
        frameHeight
    );
}

// -------------------------------------------------
// AnimationController
// -------------------------------------------------
AnimationController::AnimationController()
: currentAnimation(""),
  currentFrame(0),
  elapsedTime(0.f),
  finished(false)
{
}

// -------------------------------------------------
// Add animation
// -------------------------------------------------
void AnimationController::addAnimation(
    const std::string& name,
    const sf::Texture& texture,
    int frameCount,
    float frameTime,
    bool loop
)
{
    animations[name] = {
        &texture,
        frameCount,
        frameTime,
        loop
    };
}

// -------------------------------------------------
// Play animation
// -------------------------------------------------
void AnimationController::play(const std::string& name, bool restart)
{
    if (currentAnimation == name && !restart)
        return;

    currentAnimation = name;
    currentFrame = 0;
    elapsedTime = 0.f;
    finished = false;

    const auto& anim = animations.at(name);
    sprite.setTexture(*anim.texture);

    int frameWidth  = anim.texture->getSize().x / anim.frameCount;
    int frameHeight = anim.texture->getSize().y;

    sprite.setTextureRect(
        computeFrameRect(0, frameWidth, frameHeight)
    );

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(
        bounds.width / 2.f,
        bounds.height / 2.f
    );

}

// -------------------------------------------------
// Update animation
// -------------------------------------------------
void AnimationController::update()
{
    if (currentAnimation.empty() || finished)
        return;

    const auto& anim = animations.at(currentAnimation);

    elapsedTime += clock.restart().asSeconds();

    if (elapsedTime >= anim.frameTime)
    {
        elapsedTime = 0.f;
        currentFrame++;

        if (currentFrame >= anim.frameCount)
        {
            if (anim.loop)
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame = anim.frameCount - 1;
                finished = true;
            }
        }

        int frameWidth  = anim.texture->getSize().x / anim.frameCount;
        int frameHeight = anim.texture->getSize().y;

        sprite.setTextureRect(
            computeFrameRect(currentFrame, frameWidth, frameHeight)
        );
    }
}

// -------------------------------------------------
// State
// -------------------------------------------------
bool AnimationController::isFinished() const
{
    return finished;
}

// -------------------------------------------------
// Render helpers
// -------------------------------------------------
void AnimationController::setPosition(const sf::Vector2f& pos)
{
    sprite.setPosition(pos);
}

void AnimationController::setScale(const sf::Vector2f& scale)
{
    sprite.setScale(scale);
}

void AnimationController::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

// -------------------------------------------------
// get animation
// -------------------------------------------------
const std::string& AnimationController::getCurrentAnimation() const
{
    return currentAnimation;
}

// -------------------------------------------------
// Devuelve el frame actual
// -------------------------------------------------
int AnimationController::getCurrentFrame() const
{
    return currentFrame;
}
