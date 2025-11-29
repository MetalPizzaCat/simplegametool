#include "Error.hpp"

const char *Engine::Errors::RuntimeError::what() const throw()
{
    return nullptr;
}

Engine::Errors::RuntimeError::RuntimeError(std::string const &msg) : m_message(msg)
{
}
