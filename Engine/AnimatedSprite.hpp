#pragma once
#include <SFML/Graphics.hpp>
#include "Asset.hpp"

namespace Engine
{
    class AnimatedSprite
    {
    public:
        explicit AnimatedSprite(sf::Sprite sprite, SpriteFramesAsset const *frames);

        virtual void draw(sf::RenderWindow &window);

        virtual void update(float delta);

        void setPosition(sf::Vector2f pos);

    private:
        sf::Sprite m_sprite;
        SpriteFramesAsset const *m_frames;
        float m_animTime;
        std::string m_currentAnimationName;
        size_t m_currentAnimationFrame;
        bool m_finishedAnimation;
    };
} // namespace Engine
