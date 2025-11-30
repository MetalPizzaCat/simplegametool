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
    case ValueType::Double:
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