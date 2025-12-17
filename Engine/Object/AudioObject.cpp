#include "AudioObject.hpp"

Engine::AudioObject::AudioObject(ObjectType const *type,
                                 std::string const &name,
                                 Scene &state,
                                 std::string const &soundAssetName) : GameObject(type, name, state),
                                                                      m_sound(ContentManager::getInstance().createSoundFromAsset(ContentManager::getInstance().getSoundAsset(soundAssetName)))
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
