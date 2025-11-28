#pragma once
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <unordered_map>
#include "../Engine/Runnable.hpp"

namespace Code
{
    template <typename T>
    std::vector<uint8_t> parseToBytes(T val)
    {
        uint64_t const v = std::bit_cast<uint64_t>(val);
        std::vector<uint8_t> res;
        for (int32_t i = sizeof(T) - 1; i >= 0; i--)
        {
            uint64_t offset = (sizeof(uint8_t) * i) * 8;
            const size_t mask = 0xff;
            uint64_t num = v & (mask << offset);
            uint64_t numFixed = num >> offset;
            res.push_back((uint8_t)numFixed);
        }
        return res;
    }

    class CodeBlock
    {
    public:
        explicit CodeBlock() = default;

        void insert(std::vector<uint8_t> const &bytes);

        std::vector<uint8_t> const &getBytes() const { return m_bytes; }

    private:
        std::map<std::string, std::vector<size_t>> m_jumpLabelDestinations;
        std::map<std::string, size_t> m_jumpLabelLocations;
        std::vector<uint8_t> m_bytes;
    };

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

        CodeBlock &getCurrentBlock() { return m_blocks.back(); }

        void createBlock();

        void popBlock();

        void addFunction(std::string const &name, Engine::Runnable::RunnableFunction func);

    private:
        std::vector<CodeBlock> m_blocks;
        std::vector<std::string> m_strings;
        std::vector<Engine::Runnable::TypeInfo> m_types;
        std::map<std::string, Engine::Runnable::RunnableFunction> m_functions;
    };
}