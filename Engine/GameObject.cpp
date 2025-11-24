#include "GameObject.hpp"

void Engine::GameObject::setPosition(sf::Vector2f pos)
{
    m_position = pos;
    m_sprite.setPosition(pos);
}