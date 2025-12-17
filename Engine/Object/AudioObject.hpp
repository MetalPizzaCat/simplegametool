#pragma once

#include "GameObject.hpp"
#include "../Content/Sound.hpp"

namespace Engine
{
    class AudioObject : public GameObject
    {
    public:
        explicit AudioObject(ObjectType const *type, std::string const &name, Scene &state, std::string const &soundAssetName);

        /// @brief Play audio sample
        void play();

        void pause();

        void stop();

        void setPosition(sf::Vector2f pos) override;

    private:
        std::unique_ptr<Sound> m_sound;
    };
}