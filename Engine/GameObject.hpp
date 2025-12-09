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
        explicit GameObject(ObjectType const *type, std::string const &name, Scene &state);
        void draw(sf::RenderWindow &window);

        virtual void setPosition(sf::Vector2f pos);

        sf::Vector2f getPosition() const { return m_position; }

        void setSize(sf::Vector2f size);

        sf::Vector2f getSize() const { return m_size; }

        virtual void update(float delta);

        std::string const &getName() const { return m_name; }

        ObjectType const *getType() const { return m_type; }

        bool hasField(std::string const &name) const { return m_fields.contains(name); }

        std::optional<Value> getFieldValue(std::string const &name) const;

        void setFieldValue(std::string const &name, Value const &val);

        bool hasAnimationJustFinished() const { return m_hasAnimationJustFinished; }

        void setAnimationJustFinished(bool has) { m_hasAnimationJustFinished = has; }

        virtual ~GameObject() = default;

    protected:
        void spriteAnimationFinishedCallback();

    private:
        std::string m_name;
        std::unique_ptr<AnimatedSprite> m_sprite;
        ObjectType const *m_type;
        sf::Vector2f m_position;
        sf::Vector2f m_size;
        bool m_visible;
        std::unordered_map<std::string, Value> m_fields;
        bool m_hasAnimationJustFinished = false;
    };
}