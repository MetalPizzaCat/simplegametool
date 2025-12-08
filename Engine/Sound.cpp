#include "Sound.hpp"

Engine::Sound::Sound(sf::Sound sound, const SoundAsset *data) : m_sound(sound), m_soundData(data)
{
    m_sound.setLooping(data->isLooping());
    m_sound.setSpatializationEnabled(data->isPositional());
    m_sound.setPitch(data->getPitch());
}

void Engine::Sound::play()
{
    m_sound.play();
}

void Engine::Sound::setPosition(sf::Vector2f pos)
{
    m_sound.setPosition(sf::Vector3f(pos.x, pos.y, 0));
}
