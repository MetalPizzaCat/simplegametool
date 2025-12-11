#include "ObjectType.hpp"
#include "../Scene.hpp"
Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite,
                               std::unordered_map<std::string, Engine::Runnable::CodeConstantValue> const &fields,
                               std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,

                               std::unordered_map<std::string, Runnable::RunnableFunction> const &methods)
    : m_sprite(sprite), m_fields(fields), m_constants(constants), m_methods(methods)
{
}

Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite,
                               ObjectType const *parentType,
                               std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
                               std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,

                               std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
                               std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods)
    : m_sprite(sprite), m_parent(parentType), m_fields(fields), m_constants(constants), m_methods(methods), m_nativeMethods(nativeMethods)
{
}

void Engine::ObjectType::callNativeMethod(std::string const &name, Scene &scene) const
{
    m_nativeMethods.at(name)(scene);
}

std::optional<Engine::Value> Engine::ObjectType::getConstant(std::string const &name, Scene &scene) const
{
    if (!m_constants.contains(name))
    {
        return {};
    }
    switch ((Runnable::CodeConstantValueType)m_constants.at(name).index())
    {
    case Runnable::CodeConstantValueType::Bool:
        return std::get<bool>(m_constants.at(name));
    case Runnable::CodeConstantValueType::Int:
        return std::get<int64_t>(m_constants.at(name));

    case Runnable::CodeConstantValueType::Float:
        return std::get<double>(m_constants.at(name));

    case Runnable::CodeConstantValueType::StringId:
        if (std::optional<std::string> str = scene.getConstantStringById(std::get<size_t>(m_constants.at(name))); str.has_value())
        {
            return scene.createString(str.value());
        }
        else
        {
            throw std::runtime_error("Failed to find string");
        }

    case Runnable::CodeConstantValueType::Vector:
        return std::get<sf::Vector2f>(m_constants.at(name));
    }
    return {};
}
