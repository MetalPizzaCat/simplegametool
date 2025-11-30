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
        Int,
        Double,
        Vector,
        Object,
        String,
    };
    class GameObject;
    class StringObject;
    using Value = std::variant<bool, int64_t, double, sf::Vector2f, GameObject *, StringObject *>;

    std::string valueToString(Value const& v);

}