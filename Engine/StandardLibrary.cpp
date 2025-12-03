#include "StandardLibrary.hpp"
#include "Error.hpp"
#include <cmath>
void Engine::Standard::sqrt(Scene &scene)
{
    Value a = scene.popFromStackOrError();
    if (a.index() != ValueType::Double)
    {
        throw Errors::RuntimeMemoryError("Sqrt only accepts float");
    }
    scene.pushToStack(std::sqrt(std::get<double>(a)));
}