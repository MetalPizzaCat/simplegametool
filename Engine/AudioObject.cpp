#include "AudioObject.hpp"

Engine::AudioObject::AudioObject(ObjectType const *type,
                                 std::string const &soundAssetName,
                                 std::string const &name,
                                 Scene &state) : GameObject(type, name, state),
                                                 m_sound(ContentManager::getInstance().loadSound(ContentManager::getInstance().getSoundAsset(soundAssetName)))
{
}

void Engine::AudioObject::play()
{
    m_sound->play();
}

void Engine::AudioObject::setPosition(sf::Vector2f pos)
{
    m_sound->setPosition(pos);
}
