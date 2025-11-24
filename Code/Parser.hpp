#pragma once
#include <string>

class Parser
{
public:
    explicit Parser(std::string const &code);

private:
    void advance();

    void advanceBy(size_t offset);
};