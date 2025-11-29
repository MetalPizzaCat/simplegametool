#include "ObjectType.hpp"

Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite, std::map<std::string, Engine::Runnable::CodeConstantValue> const &fields)
    : m_sprite(sprite), m_fields(fields)
{
}

Engine::ObjectType::ObjectType(SpriteFramesAsset const *sprite,
                               BaseObjectType baseType,
                               ObjectType const *parentType,
                               std::map<std::string, Runnable::CodeConstantValue> const &fields)
    : m_sprite(sprite), m_baseType(baseType), m_parent(parentType), m_fields(fields)
{
}
