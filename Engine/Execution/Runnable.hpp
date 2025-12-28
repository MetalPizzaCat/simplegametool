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
        std::string typeName;
        std::string functionName;
    };


    struct RunnableCode
    {
        Code::Debug::DebugInfo debugInfo;
        std::unordered_map<std::string, RunnableFunction> functions;
        std::vector<std::string> strings;
        //std::unordered_map<std::string, Code::Debug::DebugInfoSourceData> typeDeclarationLocations;
    };
} // namespace Engine
