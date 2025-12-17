#include "TextObject.hpp"

Engine::TextObject::TextObject(ObjectType const *type, std::string const &name, Scene &scene, std::string const &fontName)
    : GameObject(type, name, scene), m_text(*ContentManager::getInstance().getFontByAssetName(fontName))
{
}

void Engine::TextObject::draw(sf::RenderWindow &window)
{
    window.draw(m_text);
}

void Engine::TextObject::setText(std::string const &text)
{
    m_text.setString(text);
}

std::string Engine::TextObject::getText() const
{
    return m_text.getString();
}

void Engine::TextObject::setPosition(sf::Vector2f pos)
{
    GameObject::setPosition(pos);
    m_text.setPosition(pos);
}

void Engine::TextObject::setFontSize(IntType size)
{
    m_text.setCharacterSize(size);
}
