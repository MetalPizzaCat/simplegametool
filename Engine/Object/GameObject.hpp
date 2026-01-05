#pragma once

#include "ObjectType.hpp"
#include "MemoryObject.hpp"
#include "../Content/ContentManager.hpp"
#include "../Content/AnimatedSprite.hpp"

namespace Engine
{
    class Scene;
    /// @brief Object representing anything that participates in the game and has visuals or sounds attached to it.
    class GameObject : public MemoryObject
    {
    public:
        explicit GameObject(ObjectType const *type, std::string const &name, Scene &state);

        virtual void draw(sf::RenderWindow &window);

        /// @brief Set position of the object in the game space
        /// @param pos Position
        virtual void setPosition(sf::Vector2f pos);

        sf::Vector2f getPosition() const { return m_position; }

        /// @brief Set desired size of the object and scale the object sprite if present
        /// @param size Desired size
        void setSize(sf::Vector2f size);

        /// @brief Get current size of the object
        sf::Vector2f getSize() const { return m_size; }

        virtual void update(float delta);

        std::string const &getName() const { return m_name; }

        /// @brief Get current script type of the object
        /// @return Pointer to the type information
        ObjectType const *getType() const { return m_type; }

        /// @brief Check whether the given object contains a field with a given name
        /// @param name Name of the field
        /// @return True if field is present
        bool hasField(std::string const &name) const { return m_fields.contains(name); }

        /// @brief Get value of the field if present
        /// @param name Name of the field
        /// @return Value of the field or None if no field uses that name
        std::optional<Value> getFieldValue(std::string const &name) const;

        /// @brief Set value of a field in the object, if base type didn't have this field a new entry exclusive to this instance of the object will be created
        /// @param name Name of the field
        /// @param val Value to assign
        void setFieldValue(std::string const &name, Value const &val);

        /// @brief Check if animation in the sprite controlled by this object has finished on this frame
        /// @return True if animation has finished on this frame of logic
        bool hasAnimationJustFinished() const { return m_hasAnimationJustFinished; }

        void setAnimationJustFinished(bool has) { m_hasAnimationJustFinished = has; }

        /// @brief Change animated sprite to a given asset
        /// @param asset Sprite asset to update to, if `nullptr` sprite is just deleted 
        void changeSprite(SpriteFramesAsset const* asset);

        /// @brief Mark object as ready to be destroyed. This does not instantly clear the memory used by the object to avoid accessing invalid data,
        /// but it will put in the state where it is not rendered and not updated and attempting to call functions on it will throw errors
        void destroy();

        bool isDestroyed() const { return m_destroyed; }

        std::string toString() const override { return std::string("Object@") + getName(); }

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
        bool m_destroyed;
        std::unordered_map<std::string, Value> m_fields;
        bool m_hasAnimationJustFinished = false;
    };

    /// @brief Check if given object has been destroyed and throw an error if it was
    /// @param obj  Object to check
    void validateObject(GameObject const *obj);
}