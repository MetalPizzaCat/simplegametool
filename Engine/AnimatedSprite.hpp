#pragma once
#include <SFML/Graphics.hpp>
#include "Asset.hpp"
#include <functional>

namespace Engine
{
    class AnimatedSprite
    {
    public:
        explicit AnimatedSprite(sf::Sprite sprite, SpriteFramesAsset const *frames);

        void draw(sf::RenderWindow &window);

        /// @brief Update animation of the sprite
        /// @param delta Time since last frame
        void update(float delta);

        void setPosition(sf::Vector2f pos);

        /// @brief Set desired size of the sprite, scale will be adjusted based on frame size
        /// @param size Desired size
        void setSize(sf::Vector2f size);

        sf::IntRect getCurrentFrameSize() const;

        /// @brief Set function to call upon finishing the animation
        /// @param f Function
        void setAnimationFinishedCallback(std::function<void()> const &f) { m_animationFinishedCallback = std::move(f); }

    private:
        std::function<void()> m_animationFinishedCallback;
        sf::Sprite m_sprite;
        SpriteFramesAsset const *m_frames;
        float m_animTime;
        std::string m_currentAnimationName;
        size_t m_currentAnimationFrame;
        bool m_finishedAnimation;
        sf::Vector2f m_size;
    };
} // namespace Engine
