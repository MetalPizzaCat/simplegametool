#include "StandardLibrary.hpp"
#include "Error.hpp"
#include <cmath>
#include "AudioObject.hpp"
void Engine::Standard::sqrt(Scene &scene)
{
    Value a = scene.popFromStackOrError();
    if (a.index() != ValueType::Float)
    {
        throw Errors::RuntimeMemoryError("Sqrt only accepts float");
    }
    scene.pushToStack(std::sqrt(std::get<double>(a)));
}

void Engine::Standard::Audio::audioPlayerPlay(Scene &scene)
{
    GameObject *obj = scene.popFromStackAsType<GameObject *>("Expected audio object");
    if (Engine::AudioObject *audio = dynamic_cast<Engine::AudioObject *>(obj); audio != nullptr)
    {
        audio->play();
    }
}

void Engine::Standard::Audio::setListenerPosition(Scene &scene)
{
    sf::Vector2f pos = scene.popFromStackAsType<sf::Vector2f>("Expected position on stack");
    sf::Listener::setPosition(sf::Vector3f(pos.x, pos.y, 0));
}
