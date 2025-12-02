#include "ObjectType.hpp"
#include "Scene.hpp"
Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite,
                               std::map<std::string, Engine::Runnable::CodeConstantValue> const &fields,
                               std::map<std::string, Runnable::RunnableFunction> const &methods)
    : m_sprite(sprite), m_fields(fields), m_methods(methods)
{
}

Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite,
                               ObjectType const *parentType,
                               std::map<std::string, Runnable::CodeConstantValue> const &fields,
                               std::map<std::string, Runnable::RunnableFunction> const &methods,
                               std::map<std::string, std::function<void(Scene &scene)>> const &nativeMethods)
    : m_sprite(sprite), m_parent(parentType), m_fields(fields), m_methods(methods), m_nativeMethods(nativeMethods)
{
}

void Engine::ObjectType::callNativeMethod(std::string const &name, Scene &scene) const
{
    m_nativeMethods.at(name)(scene);
}
