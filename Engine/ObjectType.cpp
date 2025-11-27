#include "ObjectType.hpp"

Engine::ObjectType::ObjectType(Asset const *sprite, std::map<std::string, Engine::Runnable::CodeConstantValue> const &fields)
    : m_sprite(sprite), m_fields(fields)
{
}