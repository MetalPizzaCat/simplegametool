#pragma once
#include "Asset.hpp"
#include <map>
#include "Value.hpp"

namespace Engine
{
    class ObjectType
    {
    public:
        explicit ObjectType(Asset const *sprite, std::map<std::string, ValueType> const &fields)
        {
            m_sprite = sprite;
            m_fields = fields;
        }
        Asset const *getSpriteData() const { return m_sprite; }

        std::map<std::string, ValueType> const &getField() const { return m_fields; }

    private:
        Asset const *m_sprite;
        std::map<std::string, ValueType> m_fields;
    };
}