#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <optional>

namespace Code::Debug
{
    struct DebugInfoEntry
    {
        size_t start;
        size_t end;
        size_t row;
        size_t column;
    };

    struct DebugInfoSourceData
    {
        size_t row;
        size_t column;
    };

    class FunctionDebugInfo
    {
    public:
        explicit FunctionDebugInfo(std::string const& typeName, std::string const &name, std::string const &sourceFileName);

        std::string const& getTypeName() const { return m_typeName; }

        std::string const &getName() const { return m_functionName; }

        std::string const &getFileName() const { return m_fileName; }

        std::optional<std::pair<size_t, size_t>> getFilePositionForByte(size_t byte) const;

        void addByteRange(size_t start, size_t end, size_t row, size_t column);

        /// @brief Add byte range by using `end` value of the previous entry as start
        /// @param len How long the byte range is
        /// @param row Row in the file
        /// @param column Column in the file
        void addByteRangeFromPrevious(size_t len, size_t row, size_t column);

    private:
        std::string m_typeName;
        std::string m_fileName;
        std::string m_functionName;
        std::vector<DebugInfoEntry> m_data;
    };

    class DebugInfo
    {
    public:
        explicit DebugInfo(std::vector<FunctionDebugInfo> const &info);

        /// @brief Try to get debug info for position in file for the given byte
        /// @param typeName Type in which to search for the function information
        /// @param functionName Name of the function
        /// @param bytePos Position in bytecode
        /// @return Tuple of format `(row, column)` or none if no info found
        std::optional<std::pair<size_t, size_t>> getFilePositionForByte(std::string const &typeName, std::string const &functionName, size_t bytePos) const;

    private:
        std::vector<FunctionDebugInfo> m_info;
        std::map<std::string, DebugInfoSourceData> m_typeDeclarationLocations;
    };
} // namespace Code::Debug
