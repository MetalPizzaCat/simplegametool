#pragma once
#include <variant>
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
        Object
    };
    class GameObject;
    using Value = std::variant<bool, int64_t, double, sf::Vector2f, GameObject *>;

}