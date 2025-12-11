#pragma once
#include <SFML/Graphics.hpp>
#include <variant>
#include <string>
#include <map>
#include <cstdint>
#include "../../Code/DebugInfo.hpp"

namespace Engine::Runnable
{
    enum class CodeConstantValueType
    {
        Bool,
        Int,
        Float,
        StringId,
        Vector
    };
    using CodeConstantValue = std::variant<bool, int64_t, double, size_t, sf::Vector2f>;

    struct RunnableFunction
    {
        size_t argumentCount;
        std::vector<uint8_t> bytes;
    };

    struct RunnableFunctionDebugInfo
    {
        size_t typeId;
        std::string functionName;
    };

    class TypeInfo
    {
    public:
        /// @brief Create a new instance of TypeInfo
        /// @param name Name of the type as referenced in the code
        /// @param spritePath Path of asset that contains animation info for sprite
        /// @param fields Default fields of the type
        /// @param methods Methods that are present in the type
        /// @param defaultType if given type info refences incomplete "forward" definition of a standard type or not(types such as std, Input, etc.) or not
        explicit TypeInfo(std::string const &name,
                          std::string const &spritePath,
                          std::unordered_map<std::string, CodeConstantValue> const &fields,
                          std::unordered_map<std::string, CodeConstantValue> const &constants,
                          std::unordered_map<std::string, RunnableFunction> const &methods, bool defaultType) : m_name(name),
                                                                                                                m_spritePath(spritePath),
                                                                                                                m_fields(fields),
                                                                                                                m_constants(constants),
                                                                                                                m_methods(methods),
                                                                                                                m_isDefaultType(defaultType) {}
        std::string const &getName() const { return m_name; }

        std::unordered_map<std::string, CodeConstantValue> const &getFields() const { return m_fields; }

        std::unordered_map<std::string, CodeConstantValue> const &getConstants() const { return m_constants; }

        std::unordered_map<std::string, RunnableFunction> const &getMethods() const { return m_methods; }

        std::string const &getSpriteName() const { return m_spritePath; }

        /// @brief Check if given type info refences incomplete "forward" definition of a standard type or not(types such as std, Input, etc.)
        /// @return True if this type is for default type of the system
        bool isDefaultType() const { return m_isDefaultType; }

    private:
        std::string m_name;
        std::string m_spritePath;
        std::unordered_map<std::string, CodeConstantValue> m_fields;
        std::unordered_map<std::string, CodeConstantValue> m_constants;
        std::unordered_map<std::string, RunnableFunction> m_methods;
        bool m_isDefaultType;
    };

    struct RunnableCode
    {
        Code::Debug::DebugInfo debugInfo;
        std::unordered_map<std::string, RunnableFunction> functions;
        std::vector<TypeInfo> types;
        std::vector<std::string> strings;
        std::unordered_map<std::string, Code::Debug::DebugInfoSourceData> typeDeclarationLocations;
    };
} // namespace Engine
