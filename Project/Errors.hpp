#pragma once
#include <exception>
#include <string>

namespace Project::Errors
{
    class AssetFileError : public std::exception
    {
    public:
        const char *what() const throw() override;
        AssetFileError(std::string const &msg);

    private:
        std::string m_message;
    };
} // namespace Project::Errors
