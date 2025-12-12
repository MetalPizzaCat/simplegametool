#pragma once
#include <variant>
#include <string>
#include <cstdint>
#include <SFML/Graphics.hpp>

namespace Engine
{
    enum ValueType 
    {
        Bool,
        Integer,
        Float,
        Vector,
        Object,
        String,
    };
    using IntType = int64_t;
    using FloatType = double;
    using VectorType = sf::Vector2f;
    class GameObject;
    class StringObject;
    using Value = std::variant<bool, IntType, FloatType, VectorType, GameObject *, StringObject *>;

    std::string valueToString(Value const& v);

    const char* typeToString(ValueType type);
}