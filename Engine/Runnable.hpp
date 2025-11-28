#pragma once
#include <SFML/Graphics.hpp>
#include <variant>
#include <string>
#include <map>
#include <cstdint>

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
    class TypeInfo
    {
    public:
        explicit TypeInfo(std::string const &name,
                          std::string const &spritePath,
                          std::map<std::string, CodeConstantValue> const &fields) : m_name(name),
                                                                                    m_spritePath(spritePath),
                                                                                    m_fields(fields) {}
        std::string const &getName() const { return m_name; }

        std::map<std::string, CodeConstantValue> const &getFields() const { return m_fields; }

        std::string const &getSpriteName() const { return m_spritePath; }

    private:
        std::string m_name;
        std::string m_spritePath;
        std::map<std::string, CodeConstantValue> m_fields;
    };

    struct RunnableFunction
    {
        size_t argumentCount;
        std::vector<uint8_t> bytes;
    };
    struct RunnableCode
    {
        std::map<std::string, RunnableFunction> functions;
        std::vector<TypeInfo> types;
        std::vector<std::string> strings;
    };
} // namespace Engine
