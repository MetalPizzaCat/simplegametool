#include "Code.hpp"
#include <iostream>
Engine::Runnable::RunnableCode Code::Fusion::compileFusionString(std::string const &str, std::string filename)
{
    Parser p(str);
    std::vector<std::unique_ptr<Token>> tokens = p.parseTokens();
    FusionCodeGenerator generator(std::move(tokens), filename);
    generator.generate();

    return generator.getBuilder().getRunnableCode();
}