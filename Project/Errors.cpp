#include "Errors.hpp"

const char *Project::Errors::AssetFileError::what() const throw()
{
    return m_message.c_str();
}

Project::Errors::AssetFileError::AssetFileError(std::string const &msg) : m_message(msg)
{
}
