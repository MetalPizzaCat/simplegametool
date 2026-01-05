#include "Error.hpp"

const char *Engine::Errors::RuntimeError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::RuntimeError::RuntimeError(size_t row, size_t column, std::string const &msg) : m_row(row), m_column(column), m_message(msg)
{
}

// const char *Engine::Errors::ByteCodeRuntimeError::what() const throw()
// {
//     return m_message.c_str();
// }

// Engine::Errors::ByteCodeRuntimeError::ByteCodeRuntimeError(size_t bytePosition, std::string const &msg) : m_pos(bytePosition), m_message(msg)
// {
// }

const char *Engine::Errors::SceneCreationError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::SceneCreationError::SceneCreationError(std::string const &msg) : m_message(msg)
{
}

const char *Engine::Errors::RuntimeMemoryError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::RuntimeMemoryError::RuntimeMemoryError(std::string const &msg) : m_message(msg)
{
}

const char *Engine::Errors::ExecutionError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::ExecutionError::ExecutionError(std::string const &msg) : m_message(msg)
{
}

const char *Engine::Errors::ContentError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::ContentError::ContentError(std::string const &msg) : m_message(msg)
{
}

