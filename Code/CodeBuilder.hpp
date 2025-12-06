#pragma once
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <unordered_map>
#include "DebugInfo.hpp"
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

    struct CodeJumpInfo
    {
        /// @brief Where does the jump value need to be written
        size_t byteCodeDestination;
        /// @brief Column where the jump occurred from for error displaying purposes
        size_t column;
        /// @brief Row where the jump occurred from for error displaying purposes
        size_t row;
    };

    class CodeBlock
    {
    public:
        explicit CodeBlock() = default;

        void insert(std::vector<uint8_t> const &bytes);

        std::vector<uint8_t> const &getBytes() const { return m_bytes; }

        /// @brief Generate a row of 0s equal to the size of the jump size type
        void reserveBytesForJump();

        void addLabelPosition(std::string const &name, size_t pos);

        /// @brief Check if given label has already been added to the code
        /// @param name Name of the label
        /// @return
        bool labelExists(std::string const &name) const { return m_jumpLabelLocations.contains(name); }

        size_t addVariableName(std::string const &name);

        std::optional<size_t> getVariableId(std::string const &name) const;

        /// @brief Write correct jump info into the resevered address spaces
        void applyLabels();

        void addLabelDestination(std::string const &labelname, size_t jumpAddressBytesPosition, size_t codeColumn, size_t codeRow);

    private:
        std::map<std::string, std::vector<CodeJumpInfo>> m_jumpLabelDestinations;
        std::map<std::string, size_t> m_jumpLabelLocations;
        std::vector<uint8_t> m_bytes;
        std::vector<std::string> m_variables;
    };

    class CodeBuilder
    {
    public:
        explicit CodeBuilder(std::vector<std::string> const &defaultTypes = {});

        /// @brief Add new type info
        /// @param type Type info for user defined type
        /// @return id of newly added type or -1 if type name was already in use and as such type was not added
        size_t addType(Engine::Runnable::TypeInfo const &type);

        std::optional<size_t> getTypeByName(std::string const &name);

        /// @brief Attempt to get id for a string that contains given value, if string is not registered it gets added.
        /// @param str String to add
        /// @return Id of the string
        size_t getOrAddStringId(std::string const &str);

        Engine::Runnable::RunnableCode getRunnableCode() const;

        CodeBlock &getCurrentBlock() { return m_blocks.back(); }

        void createBlock();

        /// @brief Pop the current bytecode context block
        void popBlock();

        void addFunction(std::string const &name, Engine::Runnable::RunnableFunction func);

        Debug::FunctionDebugInfo &getOrCreateDebugEntryForFunction(size_t typeId, std::string const &functionName);

        /// @brief Attempt to add info about location of type declaration
        /// @param name Name of the type
        /// @param data File debug info
        /// @return  True if added, false if type name is already in use
        bool addTypeDeclarationLocation(std::string const &name, Debug::DebugInfoSourceData const &data);

    private:
        std::vector<CodeBlock> m_blocks;
        std::vector<std::string> m_strings;
        std::vector<Engine::Runnable::TypeInfo> m_types;
        std::unordered_map<std::string, Engine::Runnable::RunnableFunction> m_functions;
        // section for data used for debug only
        std::vector<Debug::FunctionDebugInfo> m_functionDebugInfo;
        std::unordered_map<std::string, Debug::DebugInfoSourceData> m_typeDeclarationLocations;
    };
}