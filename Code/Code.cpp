#include "Code.hpp"

Engine::Runnable::RunnableCode Code::Fusion::compileFusionString(std::string const &str)
{
    Parser p(str);
    std::vector<std::unique_ptr<Token>> tokens = p.parseTokens();
    FusionCodeGenerator generator(std::move(tokens));
    generator.generate();

    return generator.getBuilder().getRunnableCode();
}