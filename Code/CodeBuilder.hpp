#pragma once
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <unordered_map>
#include "../Engine/Runnable.hpp"

namespace Code
{

    class CodeBuilder
    {
    public:
        size_t addType(Engine::Runnable::TypeInfo const &type);

        std::optional<size_t> getTypeByName(std::string const &name);

        /// @brief Attempt to get id for a string that contains given value, if string is not registered it gets added.
        /// @param str String to add
        /// @return Id of the string
        size_t getOrAddStringId(std::string const &str);

        Engine::Runnable::RunnableCode getRunnableCode() const;

    private:
        std::vector<std::string> m_strings;
        std::vector<Engine::Runnable::TypeInfo> m_types;
    };
}