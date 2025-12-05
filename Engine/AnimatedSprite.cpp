#include "AnimatedSprite.hpp"
#include "ContentManager.hpp"

Engine::AnimatedSprite::AnimatedSprite(sf::Sprite sprite, SpriteFramesAsset const *frames) : m_sprite(sprite),
                                                                                             m_frames(frames)
{
    m_animTime = 0;
    m_currentAnimationFrame = 0;
    m_currentAnimationName = "default";
    m_finishedAnimation = false;
}

void Engine::AnimatedSprite::draw(sf::RenderWindow &window)
{
    window.draw(m_sprite);
}

void Engine::AnimatedSprite::update(float delta)
{
    if (m_finishedAnimation || !m_frames->hasAnimation(m_currentAnimationName))
    {
        return;
    }
    m_animTime += delta;
    if (m_animTime > 1.0f / m_frames->getFrames(m_currentAnimationName).framesPerSecond)
    {
        m_animTime = 0;
        m_currentAnimationFrame++;
        Engine::SpriteAnimation const &frames = m_frames->getFrames(m_currentAnimationName);
        if (m_currentAnimationFrame >= frames.frames.size())
        {
            if (frames.looping)
            {
                m_currentAnimationFrame = 0;
            }
            else
            {
                m_finishedAnimation = true;
                if (m_animationFinishedCallback)
                {
                    m_animationFinishedCallback();
                }
                return;
            }
        }

        m_sprite.setTextureRect(frames.frames[m_currentAnimationFrame]);
    }
}

void Engine::AnimatedSprite::setPosition(sf::Vector2f pos)
{
    m_sprite.setPosition(pos);
}