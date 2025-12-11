#pragma once
#include "Parser.hpp"
#include "CodeGenerator.hpp"
#include "../Engine/Execution/Runnable.hpp"
namespace Code
{
    namespace Fusion
    {
        Engine::Runnable::RunnableCode compileFusionString(std::string const &str);
    }
}