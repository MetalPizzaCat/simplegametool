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

    class InvalidInstructionError : public std::exception
    {
    public:
        const char *what() const throw() override;
        InvalidInstructionError(std::string const &msg, size_t pos);

        size_t getPosition() const {return m_pos;}

    private:
        std::string m_message;
        size_t m_pos;
    };
}