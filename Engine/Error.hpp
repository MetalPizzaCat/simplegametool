#pragma once
#include <string>
#include <cstdint>
#include <exception>

namespace Engine::Errors
{

    class RuntimeMemoryError : public std::exception
    {
    public:
        const char *what() const throw() override;
        RuntimeMemoryError(std::string const &msg);


    private:
        std::string m_message;
    };

    /// @brief Alternative error type that doesn't use any of the debug info
    class ExecutionError : public std::exception
    {
    public:
        const char *what() const throw() override;
        ExecutionError(std::string const &msg);


    private:
        std::string m_message;
    };

    // class ByteCodeRuntimeError : public std::exception
    // {
    // public:
    //     const char *what() const throw() override;
    //     ByteCodeRuntimeError(size_t bytePosition, std::string const &msg);

    //     size_t getPosition() const { return m_pos; }

    // private:
    //     std::string m_message;
    //     size_t m_pos;
    // };

    class RuntimeError : public std::exception
    {
    public:
        const char *what() const throw() override;
        RuntimeError(size_t row, size_t column, std::string const &msg);

        size_t getLine() const { return m_row; }
        size_t getColumn() const { return m_column; }

    private:
        std::string m_message;
        size_t m_row;
        size_t m_column;
    };

    class InvalidInstructionError : public std::exception
    {
    public:
        const char *what() const throw() override;
        InvalidInstructionError(std::string const &msg, size_t pos);

        size_t getPosition() const { return m_pos; }

    private:
        std::string m_message;
        size_t m_pos;
    };
}