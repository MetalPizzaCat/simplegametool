#pragma once
#include "Asset.hpp"
#include <map>
#include <functional>
#include "Value.hpp"
#include "Runnable.hpp"

namespace Engine
{

    class Scene;

    class ObjectType
    {
    public:
        explicit ObjectType(SpriteFramesAsset const *sprite,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::unordered_map<std::string, Runnable::RunnableFunction> const &methods);

        explicit ObjectType(SpriteFramesAsset const *sprite,
                            ObjectType const *parentType,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                            std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods);
        SpriteFramesAsset const *getSpriteData() const { return m_sprite; }

        std::unordered_map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

        bool hasMethod(std::string const &name) const { return m_methods.contains(name); }

        bool isNativeMethod(std::string const &name) const { return m_nativeMethods.contains(name); }

        Runnable::RunnableFunction const &getMethod(std::string const &name) const { return m_methods.at(name); }

        void callNativeMethod(std::string const &name, Scene &scene) const;

    private:
        std::unordered_map<std::string, Runnable::RunnableFunction> m_methods;
        SpriteFramesAsset const *m_sprite;
        ObjectType const *m_parent;
        std::unordered_map<std::string, Runnable::CodeConstantValue> m_fields;
        std::unordered_map<std::string, std::function<void(Scene &scene)>> m_nativeMethods;
    };

}