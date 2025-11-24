#pragma once

#include "ObjectType.hpp"
#include "ContentManager.hpp"

namespace Engine
{
    class GameObject
    {
    public:
        explicit GameObject(ObjectType const *type) : m_sprite(*ContentManager::getInstance().loadTexture(type->getSpriteData()->getPath()))
        {
            assert(type != nullptr);
            ;
        }
        void draw(sf::RenderWindow &window) { window.draw(m_sprite); }

        void setPosition(sf::Vector2f pos);
        sf::Vector2f getPosition() const { return m_position; }

    private:
        sf::Sprite m_sprite;
        ObjectType const *m_type;
        sf::Vector2f m_position;
        bool m_visible;
    };
}