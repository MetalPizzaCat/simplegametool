#pragma once
#include "Parser.hpp"
#include "CodeGenerator.hpp"
#include "../Engine/Execution/Runnable.hpp"
namespace Code
{
    namespace Fusion
    {
        /// @brief Compile provided source code into runnable code
        /// @param str Code to process and compile
        /// @param filename Optional name of the file from which the code came to help with debugging, otherwise assume all code came from same file
        /// @return Structure containing data required to run the code
        Engine::Runnable::RunnableCode compileFusionString(std::string const &str, std::string filename = "");
    }
}