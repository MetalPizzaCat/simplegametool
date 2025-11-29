#pragma once

#include "ObjectType.hpp"
#include "ContentManager.hpp"
#include "AnimatedSprite.hpp"

namespace Engine
{
    class Scene;
    class GameObject
    {
    public:
        explicit GameObject(ObjectType const *type, Scene &state);
        void draw(sf::RenderWindow &window);

        void setPosition(sf::Vector2f pos);
        sf::Vector2f getPosition() const { return m_position; }

        virtual void update(float delta);
        virtual ~GameObject() = default;

    private:
        std::unique_ptr<AnimatedSprite> m_sprite;
        ObjectType const *m_type;
        sf::Vector2f m_position;
        bool m_visible;
        std::map<std::string, Value> m_fields;
    };
}