#include "GameObject.hpp"
#include "../Scene.hpp"
Engine::GameObject::GameObject(ObjectType const *type, std::string const &name, Scene &state)
    : m_sprite(std::move(ContentManager::getInstance().loadSprite(type->getSpriteData()))), m_type(type), m_name(name), m_visible(true)
{
    for (std::pair<std::string, Runnable::CodeConstantValue> const &val : type->getFields())
    {
        switch ((Runnable::CodeConstantValueType)val.second.index())
        {
        case Runnable::CodeConstantValueType::Bool:
            m_fields[val.first] = std::get<bool>(val.second);
            break;
        case Runnable::CodeConstantValueType::Int:
            m_fields[val.first] = std::get<int64_t>(val.second);
            break;
        case Runnable::CodeConstantValueType::Float:
            m_fields[val.first] = std::get<double>(val.second);
            break;
        case Runnable::CodeConstantValueType::StringId:
            if (std::optional<std::string> str = state.getConstantStringById(std::get<size_t>(val.second)); str.has_value())
            {
                m_fields[val.first] = state.createString(str.value());
            }
            else
            {
                throw std::runtime_error("Failed to find string");
            }
            break;
        case Runnable::CodeConstantValueType::Vector:
            m_fields[val.first] = std::get<sf::Vector2f>(val.second);
            break;
        }
    }
    if (m_sprite)
    {
        m_sprite->setAnimationFinishedCallback(std::bind(&GameObject::spriteAnimationFinishedCallback, this));
        m_size = sf::Vector2f(m_sprite->getCurrentFrameSize().size.x, m_sprite->getCurrentFrameSize().size.y);
    }
}

void Engine::GameObject::draw(sf::RenderWindow &window)
{
    if (m_visible)
    {
        if (m_sprite != nullptr)
        {
            m_sprite->draw(window);
        }
    }
}

void Engine::GameObject::setPosition(sf::Vector2f pos)
{
    m_position = pos;
    if (m_sprite != nullptr)
    {
        m_sprite->setPosition(pos);
    }
}

void Engine::GameObject::setSize(sf::Vector2f size)
{
    m_size = size;
    m_sprite->setSize(size);
}

void Engine::GameObject::update(float delta)
{
    if (m_sprite != nullptr)
    {
        m_sprite->update(delta);
    }
}

std::optional<Engine::Value> Engine::GameObject::getFieldValue(std::string const &name) const
{
    if (m_fields.contains(name))
    {
        return m_fields.at(name);
    }
    return {};
}

void Engine::GameObject::setFieldValue(std::string const &name, Value const &val)
{
    m_fields[name] = val;
}

void Engine::GameObject::spriteAnimationFinishedCallback()
{
    m_hasAnimationJustFinished = true;
}
