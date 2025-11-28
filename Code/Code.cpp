#include "Code.hpp"
#include <iostream>
Engine::Runnable::RunnableCode Code::Fusion::compileFusionString(std::string const &str)
{
    Parser p(str);
    std::vector<std::unique_ptr<Token>> tokens = p.parseTokens();
    for (auto const &t : tokens)
    {
        std::cout << t->toString() << " ";
    }
    std::cout << std::endl;
    FusionCodeGenerator generator(std::move(tokens));
    generator.generate();

    return generator.getBuilder().getRunnableCode();
}