#pragma once
#include <string>
#include <cstdint>
#include <exception>

namespace Code::Errors
{
    class ParsingError : public std::exception
    {
    public:
        const char *what() const throw() override;
        ParsingError(size_t line, size_t column, std::string const &msg);

        ParsingError(std::string const &msg);

        size_t getLine() const { return m_line; }
        size_t getColumn() const { return m_column; }

    private:
        size_t m_line;
        size_t m_column;
        std::string m_message;
    };
}