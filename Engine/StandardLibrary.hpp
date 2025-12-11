#pragma once
#include "Scene.hpp"

namespace Engine::Standard
{
    void sqrt(Scene &scene);

    namespace Input
    {
        void isKeyPressed(Scene &scene);

        void isMouseButtonPressed(Scene &scene);

        void getMousePosition(Scene &scene);

    }

    namespace Random
    {

        void seed(Scene &scene);

        void getRandomIntInRange(Scene &scene);

        void getRandomFloatInRange(Scene &scene);
    }

    namespace Audio
    {
        void audioPlayerPlay(Scene &scene);

        void setListenerPosition(Scene &scene);
    }
} // namespace Engine::Standard
