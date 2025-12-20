#include "StandardLibrary.hpp"
#include "../Error.hpp"
#include <cmath>
#include "../Object/AudioObject.hpp"
#include "../Object/TextObject.hpp"
#include "Random.hpp"

// TODO: Replace with better globally available system

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
    if (Engine::AudioObject *audio = dynamic_cast<Engine::AudioObject *>(scene.popFromStackAsType<GameObject *>("Expected audio object")); audio != nullptr)
    {
        Engine::validateObject(audio);
        audio->play();
    }
}

void Engine::Standard::Audio::setListenerPosition(Scene &scene)
{
    sf::Vector2f pos = scene.popFromStackAsType<sf::Vector2f>("Expected position on stack");
    sf::Listener::setPosition(sf::Vector3f(pos.x, pos.y, 0));
}

void Engine::Standard::Input::isKeyPressed(Scene &scene)
{
    int64_t scancode = scene.popFromStackAsType<int64_t>("Expected scancode on stack");
    scene.pushToStack(sf::Keyboard::isKeyPressed((sf::Keyboard::Scancode)scancode));
}

void Engine::Standard::Random::seed(Scene &scene)
{
    int64_t seed = scene.popFromStackAsType<int64_t>("Expected seed value on stack");
    Engine::Random::getInstance().seed(seed);
}

void Engine::Standard::Random::getRandomIntInRange(Scene &scene)
{
    int64_t max = scene.popFromStackAsType<int64_t>("Expected max value on stack");
    int64_t min = scene.popFromStackAsType<int64_t>("Expected min value on stack");
    scene.pushToStack(Engine::Random::getInstance().getRandomIntInRange(min, max));
}

void Engine::Standard::Random::getRandomFloatInRange(Scene &scene)
{
    double max = scene.popFromStackAsType<double>("Expected max value on stack");
    double min = scene.popFromStackAsType<double>("Expected min value on stack");
    scene.pushToStack(Engine::Random::getInstance().getRandomFloatInRange(min, max));
}

void Engine::Standard::Label::setText(Scene &scene)
{

    if (TextObject *txt = dynamic_cast<TextObject *>(scene.popFromStackAsType<GameObject *>("Expected label on stack")); txt != nullptr)
    {

        std::string const &str = scene.popFromStackAsType<StringObject *>("Expected string on stack")->getString();
        txt->setText(str);
    }
    else
    {
        throw Errors::RuntimeMemoryError("Expected label on stack but got wrong type");
    }
}

void Engine::Standard::Label::getText(Scene &scene)
{
    if (TextObject *txt = dynamic_cast<TextObject *>(scene.popFromStackAsType<GameObject *>("Expected label on stack")); txt != nullptr)
    {

        scene.pushToStack(scene.createString(txt->getText()));
    }
    else
    {
        throw Errors::RuntimeMemoryError("Expected label on stack but got wrong type");
    }
}

void Engine::Standard::Label::setFontSize(Scene &scene)
{

    if (TextObject *txt = dynamic_cast<TextObject *>(scene.popFromStackAsType<GameObject *>("Expected label on stack")); txt != nullptr)
    {

        IntType size = scene.popFromStackAsType<IntType>("Expected font size as int on stack");
        txt->setFontSize(size);
    }
    else
    {
        throw Errors::RuntimeMemoryError("Expected label on stack but got wrong type");
    }
}
