#include "Error.hpp"

const char *Code::Errors::ParsingError::what() const throw()
{
    return m_message.c_str();
}

Code::Errors::ParsingError::ParsingError(size_t line, size_t column, std::string const &msg) : m_message(msg), m_column(column), m_line(line)
{
}

Code::Errors::ParsingError::ParsingError(std::string const &msg) : m_message(msg), m_column((size_t)-1), m_line((size_t)-1)
{
}

const char *Code::Errors::MultipleTypeDefinitionError::what() const throw()
{
    return m_message.c_str();
}

Code::Errors::MultipleTypeDefinitionError::MultipleTypeDefinitionError(std::string const &msg) : m_message(msg)
{
}
