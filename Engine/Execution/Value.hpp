#pragma once
#include <variant>
#include <string>
#include <cstdint>
#include <SFML/Graphics.hpp>

namespace Engine
{
    enum ValueType
    {
        Nil,
        Bool,
        Integer,
        Float,
        Vector,
        Object,
        String,
        Array
    };
    // Declare some types aliases to make changeing underlying types easier
    using IntType = int64_t;
    using FloatType = double;
    using VectorType = sf::Vector2f;
    using NilType = std::monostate;
    // Predeclare classes to avoid having them included because we get circular inclusion otherwise
    class GameObject;
    class StringObject;
    class ArrayObject;

    static const NilType NilValue = NilType();
    /// @brief Special type containing all possible values that can be used in the engine
    using Value = std::variant<NilType, bool, IntType, FloatType, VectorType, GameObject *, StringObject *, ArrayObject *>;

    /// @brief Get string representation of the given value
    /// @param v Value to convert to string
    std::string valueToString(Value const &v);

    /// @brief Get human readable string representing type name
    /// @param type Type
    const char *typeToString(ValueType type);

    /// @brief Increase reference count for value if value if refcounted, otherwise do nothing
    /// @param v Value
    void increaseValueRefCount(Value const &v);

    /// @brief Decrease reference count for value if value if refcounted, otherwise do nothing
    /// @param v Value
    void decreaseValueRefCount(Value const &v);
}