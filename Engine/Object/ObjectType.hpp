#pragma once
#include <map>
#include <functional>
#include "../Content/Asset.hpp"
#include "../Execution/Value.hpp"
#include "../Execution/Runnable.hpp"

namespace Engine
{

    class Scene;

    /// @brief Class containing information related to user defined types. Stores all the information about methods, fields and constants that get copied into instances of objects using the type
    class ObjectType
    {
    public:
        /// @brief Create a new instance of the object type
        /// @param name Name of the type
        /// @param sprite Sprite animation asset reference
        /// @param parentType Base type that this inherits from(currently unused)
        /// @param fields All the fields with default values which new instance of this type will have
        /// @param constants Constants of a given type
        /// @param methods 
        /// @param nativeMethods 
        /// @param strings 
        explicit ObjectType(std::string const &name,
                            SpriteFramesAsset const *sprite,
                            ObjectType const *parentType,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                            std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
                            std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                            std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods,
                            std::vector<std::string> const &strings = {});
        SpriteFramesAsset const *getSpriteData() const { return m_sprite; }

        std::unordered_map<std::string, Runnable::CodeConstantValue> const &getFields() const { return m_fields; }

        bool hasMethod(std::string const &name) const { return m_methods.contains(name); }

        bool isNativeMethod(std::string const &name) const { return m_nativeMethods.contains(name); }

        Runnable::RunnableFunction const &getMethod(std::string const &name) const { return m_methods.at(name); }

        void callNativeMethod(std::string const &name, Scene &scene) const;

        inline std::string const &getStringAt(size_t id) const { return m_strings.at(id); }

        bool hasStringAt(size_t id) const { return id < m_strings.size(); }

        std::string const &getName() const { return m_name; }

        /// @brief Try getting the constant field
        /// @param name Name of the field to get
        /// @param scene Scene used for resolving strings, if constant references a string scene will be used to create string object
        /// @return Value containing constant or none if no constant uses that name
        std::optional<Value> getConstant(std::string const &name, Scene &scene) const;

    private:
        std::string m_name;
        std::unordered_map<std::string, Runnable::RunnableFunction> m_methods;
        SpriteFramesAsset const *m_sprite;
        ObjectType const *m_parent;
        std::unordered_map<std::string, Runnable::CodeConstantValue> m_fields;
        std::unordered_map<std::string, Runnable::CodeConstantValue> m_constants;
        std::vector<std::string> m_strings;
        std::unordered_map<std::string, std::function<void(Scene &scene)>> m_nativeMethods;
    };

}