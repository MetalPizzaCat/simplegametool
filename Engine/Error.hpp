#pragma once
#include <string>
#include <cstdint>
#include <exception>

namespace Engine::Errors
{
    class RuntimeError : public std::exception
    {
    public:
        const char *what() const throw() override;
        RuntimeError(std::string const &msg);
    private:
        std::string m_message;
    };
}