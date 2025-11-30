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
        explicit ObjectType(SpriteFramesAsset const *sprite,
                            std::map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::map<std::string, Runnable::RunnableFunction> const &methods);

        explicit ObjectType(SpriteFramesAsset const *sprite,
                            BaseObjectType baseType,
                            ObjectType const *parentType,
                            std::map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::map<std::string, Runnable::RunnableFunction> const &methods);
        SpriteFramesAsset const *getSpriteData() const { return m_sprite; }

        std::map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

        bool hasMethod(std::string const &name) const { return m_methods.contains(name); }

        Runnable::RunnableFunction const &getMethod(std::string const &name) const { return m_methods.at(name); }

    private:
        std::map<std::string, Runnable::RunnableFunction> m_methods;
        SpriteFramesAsset const *m_sprite;
        BaseObjectType m_baseType;
        ObjectType const *m_parent;
        std::map<std::string, Runnable::CodeConstantValue> m_fields;
    };

}