#include "Value.hpp"
#include "GameObject.hpp"
#include "MemoryObject.hpp"

std::string Engine::valueToString(Value const &v)
{
    switch (v.index())
    {
    case ValueType::Bool:
        return std::string(std::get<bool>(v) ? "true" : "false");
    case ValueType::Int:
        return std::to_string(std::get<int64_t>(v));
    case ValueType::Float:
        return std::to_string(std::get<double>(v));
    case ValueType::Vector:
        return std::string("(") + std::to_string(std::get<sf::Vector2f>(v).x) + "," + std::to_string(std::get<sf::Vector2f>(v).y) + ")";
    case ValueType::Object:
        return std::string("Object@") + std::get<GameObject *>(v)->getName();
    case ValueType::String:
        return std::get<StringObject *>(v)->getString();
    }
    return "INVALID DATA TYPE";
}

const char *Engine::typeToString(ValueType type)
{
    switch (type)
    {
    case ValueType::Bool:
        return "Bool";
    case ValueType::Int:
        return "Int";
    case ValueType::Float:
        return "Float";
    case ValueType::Vector:
        return "Vector";
    case ValueType::Object:
        return "Object";
    case ValueType::String:
        return "String";
    }
    return "INVALID DATA TYPE";
}
