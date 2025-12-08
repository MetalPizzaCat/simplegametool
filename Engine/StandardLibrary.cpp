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
