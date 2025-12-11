#pragma once
#include <map>
#include <functional>
#include "../Content/Asset.hpp"
#include "../Execution/Value.hpp"
#include "../Execution/Runnable.hpp"

namespace Engine
{

    class Scene;

    class ObjectType
    {
    public:
        explicit ObjectType(SpriteFramesAsset const *sprite,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
                            std::unordered_map<std::string, Runnable::RunnableFunction> const &methods);

        explicit ObjectType(SpriteFramesAsset const *sprite,
                            ObjectType const *parentType,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
                            std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                            std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods);
        SpriteFramesAsset const *getSpriteData() const { return m_sprite; }

        std::unordered_map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

        bool hasMethod(std::string const &name) const { return m_methods.contains(name); }

        bool isNativeMethod(std::string const &name) const { return m_nativeMethods.contains(name); }

        Runnable::RunnableFunction const &getMethod(std::string const &name) const { return m_methods.at(name); }

        void callNativeMethod(std::string const &name, Scene &scene) const;

        /// @brief Try getting the constant field 
        /// @param name Name of the field to get 
        /// @param scene Scene used for resolving strings, if constant references a string scene will be used to create string object
        /// @return Value containing constant or none if no constant uses that name
        std::optional<Value> getConstant(std::string const &name, Scene &scene) const;

    private:
        std::unordered_map<std::string, Runnable::RunnableFunction> m_methods;
        SpriteFramesAsset const *m_sprite;
        ObjectType const *m_parent;
        std::unordered_map<std::string, Runnable::CodeConstantValue> m_fields;
        std::unordered_map<std::string, Runnable::CodeConstantValue> m_constants;
        std::unordered_map<std::string, std::function<void(Scene &scene)>> m_nativeMethods;
    };

}