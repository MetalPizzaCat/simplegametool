#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Object/ObjectType.hpp"

namespace Engine
{
    class TypeError : public std::exception
    {
    public:
        const char *what() const throw() override;
        TypeError(std::string const &msg);

    private:
        std::string m_message;
    };

    /// @brief Manager for all loaded code types
    class TypeManager
    {
    public:
        TypeManager(TypeManager const &c) = delete;
        void operator=(TypeManager const &c) = delete;
        explicit TypeManager();
        static TypeManager &getInstance()
        {
            static TypeManager manager;
            return manager;
        }
        /// @brief Try getting a type with given name
        /// @param name Name of the type
        /// @return Pointer to the type or null if no type with given name exists
        ObjectType const *getType(std::string const &name) const;

        bool doesTypeWithNameExist(std::string const &name) const;

        /// @brief Create a type with a given name that isn't meant to be instantiated or throw `TypeError` if name is already in use
        /// @param name Name of the type
        /// @param constants Constants to add to the type
        /// @param methods Methods available in the type
        /// @param nativeMethods Native(c++) methods available in the type
        /// @return Pointer to the newly created type
        ObjectType const *createStaticType(std::string const &name,
                                           std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
                                           std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                                           std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods);

        /// @brief Create a new full type or throw `TypeError` if name is already in use
        /// @param name Name of the type
        /// @param sprite Sprite asset to use by the instances
        /// @param parentType Base type(unused)
        /// @param fields All the fields with default values that instances should have
        /// @param constants All the constants available in types
        /// @param methods All the methods present in type
        /// @param nativeMethods All the native methods present in type
        /// @param strings Strings that are referenced by the type and type's method
        /// @return Pointer to the newly created type
        ObjectType const *createType(std::string const &name,
                                     SpriteFramesAsset const *sprite,
                                     ObjectType const *parentType,
                                     std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                                     std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
                                     std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                                     std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods = {},
                                     std::vector<std::string> const &strings = {});

        void addType(std::unique_ptr<ObjectType> type);

    private:
        std::vector<std::unique_ptr<ObjectType>> m_types;
    };
} // namespace Engine
