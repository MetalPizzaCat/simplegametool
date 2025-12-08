#pragma once
#include "Asset.hpp"
#include <SFML/Audio.hpp>
namespace Engine
{
    class Sound
    {
    public:
        explicit Sound(sf::Sound sound, SoundAsset const *data);

        void play();

        void setPosition(sf::Vector2f pos);
        
    private:
        sf::Sound m_sound;
        SoundAsset const *m_soundData;
    };
} // namespace Engine
