#include "Error.hpp"

const char *Engine::Errors::RuntimeError::what() const throw()
{
    return nullptr;
}

Engine::Errors::RuntimeError::RuntimeError(std::string const &msg) : m_message(msg)
{
}

const char *Engine::Errors::InvalidInstructionError::what() const throw()
{
    return m_message.c_str();
}

Engine::Errors::InvalidInstructionError::InvalidInstructionError(std::string const &msg, size_t pos) : m_pos(pos)
{
}
