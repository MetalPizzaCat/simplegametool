#include "DebugInfo.hpp"
#include <optional>
#include <algorithm>

Code::Debug::FunctionDebugInfo::FunctionDebugInfo(std::string const &typeName,
                                                  std::string const &name,
                                                  std::string const &sourceFileName) : m_typeName(typeName),
                                                                                       m_functionName(name),
                                                                                       m_fileName(sourceFileName)
{
}

std::optional<std::pair<size_t, size_t>> Code::Debug::FunctionDebugInfo::getFilePositionForByte(size_t byte) const
{
    std::vector<DebugInfoEntry>::const_iterator it = std::find_if(
        m_data.begin(),
        m_data.end(),
        [byte](DebugInfoEntry const &entry)
        {
            return byte < entry.end && byte >= entry.start;
        });
    if (it == m_data.end())
    {
        return {};
    }
    return std::make_pair(it->row, it->column);
}

void Code::Debug::FunctionDebugInfo::addByteRange(size_t start, size_t end, size_t row, size_t column)
{
    m_data.push_back(DebugInfoEntry{.start = start, .end = end, .row = row, .column = column});
}

void Code::Debug::FunctionDebugInfo::addByteRangeFromPrevious(size_t len, size_t row, size_t column)
{
    if (m_data.empty())
    {
        m_data.push_back(DebugInfoEntry{.start = 0, .end = len, .row = row, .column = column});
    }
    else
    {
        m_data.push_back(DebugInfoEntry{.start = m_data.back().end, .end = m_data.back().end + len, .row = row, .column = column});
    }
}

Code::Debug::DebugInfo::DebugInfo(std::vector<FunctionDebugInfo> const &info) : m_info(info)
{
}

std::optional<std::pair<size_t, size_t>> Code::Debug::DebugInfo::getFilePositionForByte(std::string const &typeName, std::string const &functionName, size_t bytePos) const
{
    std::vector<FunctionDebugInfo>::const_iterator it = std::find_if(
        m_info.begin(),
        m_info.end(),
        [typeName, functionName](FunctionDebugInfo const &info)
        {
            return info.getTypeName() == typeName && info.getName() == functionName;
        });
    if (it == m_info.end())
    {
        return {};
    }

    return it->getFilePositionForByte(bytePos);
}
