#include "Value.hpp"
#include "../Object/GameObject.hpp"
#include "../Object/MemoryObject.hpp"
#include "../Object/ArrayObject.hpp"

std::string Engine::valueToString(Value const &v)
{
    switch (v.index())
    {
    case ValueType::Nil:
        return "Nil";
    case ValueType::Bool:
        return std::string(std::get<bool>(v) ? "true" : "false");
    case ValueType::Integer:
        return std::to_string(std::get<int64_t>(v));
    case ValueType::Float:
        return std::to_string(std::get<double>(v));
    case ValueType::Vector:
        return std::string("(") + std::to_string(std::get<sf::Vector2f>(v).x) + "," + std::to_string(std::get<sf::Vector2f>(v).y) + ")";
    case ValueType::Object:
    {
        GameObject *o = std::get<GameObject *>(v);
        if (o->isDestroyed())
        {
            return "Freed object";
        }
        return std::string("Object@") + std::get<GameObject *>(v)->getName();
    }

    case ValueType::String:
        return std::get<StringObject *>(v)->toString();
    case ValueType::Array:
        return std::get<ArrayObject *>(v)->toString();
    }
    return "INVALID DATA TYPE";
}

const char *Engine::typeToString(ValueType type)
{
    switch (type)
    {
    case ValueType::Nil:
        return "Nil";
    case ValueType::Bool:
        return "Bool";
    case ValueType::Integer:
        return "Int";
    case ValueType::Float:
        return "Float";
    case ValueType::Vector:
        return "Vector";
    case ValueType::Object:
        return "Object";
    case ValueType::String:
        return "String";
    case ValueType::Array:
        return "Array";
    }
    return "INVALID DATA TYPE";
}

void Engine::increaseValueRefCount(Value const &v)
{
    if (v.index() == ValueType::String)
    {
        std::get<StringObject *>(v)->increaseRefCounter();
    }
    if (v.index() == ValueType::Object)
    {
        std::get<GameObject *>(v)->increaseRefCounter();
    }
    else if (v.index() == ValueType::Array)
    {
        std::get<ArrayObject *>(v)->increaseRefCounter();
    }
}

void Engine::decreaseValueRefCount(Value const &v)
{
    if (v.index() == ValueType::String)
    {
        std::get<StringObject *>(v)->decreaseRefCounter();
    }
    if (v.index() == ValueType::Object)
    {
        std::get<GameObject *>(v)->decreaseRefCounter();
    }
    else if (v.index() == ValueType::Array)
    {
        std::get<ArrayObject *>(v)->decreaseRefCounter();
    }
}
