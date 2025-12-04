#pragma once
#include <SFML/Graphics.hpp>
#include <variant>
#include <string>
#include <map>
#include <cstdint>
#include "../Code/DebugInfo.hpp"

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
        explicit TypeInfo(std::string const &name,
                          std::string const &spritePath,
                          std::unordered_map<std::string, CodeConstantValue> const &fields,
                          std::unordered_map<std::string, RunnableFunction> const &methods) : m_name(name),
                                                                                    m_spritePath(spritePath),
                                                                                    m_fields(fields),
                                                                                    m_methods(methods) {}
        std::string const &getName() const { return m_name; }

        std::unordered_map<std::string, CodeConstantValue> const &getFields() const { return m_fields; }

        std::unordered_map<std::string, RunnableFunction> const &getMethods() const { return m_methods; }

        std::string const &getSpriteName() const { return m_spritePath; }

    private:
        std::string m_name;
        std::string m_spritePath;
        std::unordered_map<std::string, CodeConstantValue> m_fields;
        std::unordered_map<std::string, RunnableFunction> m_methods;
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
