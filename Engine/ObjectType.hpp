#pragma once
#include "Asset.hpp"
#include <map>
#include "Value.hpp"
#include "Runnable.hpp"

namespace Engine
{
    class ObjectType
    {
    public:
        explicit ObjectType(Asset const *sprite, std::map<std::string, Runnable::CodeConstantValue> const &fields);
        Asset const *getSpriteData() const { return m_sprite; }

        std::map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

    private:
        Asset const *m_sprite;
        std::map<std::string, Runnable::CodeConstantValue> m_fields;
    };
}