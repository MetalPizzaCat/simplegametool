#pragma once
#include "Asset.hpp"
#include <map>
#include "Value.hpp"
#include "Runnable.hpp"

namespace Engine
{
    enum class BaseObjectType
    {
        Object,
        Button,
        Label,
        Input,
    };

    class ObjectType
    {
    public:
        explicit ObjectType(SpriteFramesAsset const *sprite, std::map<std::string, Runnable::CodeConstantValue> const &fields);

        explicit ObjectType(SpriteFramesAsset const *sprite, BaseObjectType baseType, ObjectType const *parentType, std::map<std::string, Runnable::CodeConstantValue> const &fields);
        SpriteFramesAsset const *getSpriteData() const { return m_sprite; }

        std::map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

    private:
        SpriteFramesAsset const *m_sprite;
        BaseObjectType m_baseType;
        ObjectType const *m_parent;
        std::map<std::string, Runnable::CodeConstantValue> m_fields;
    };

}