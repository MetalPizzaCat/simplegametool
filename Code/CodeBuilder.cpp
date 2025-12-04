#include "CodeBuilder.hpp"
#include <algorithm>
#include "../Engine/Instructions.hpp"
#include "Error.hpp"
Code::CodeBuilder::CodeBuilder() : m_types({Engine::Runnable::TypeInfo("std", "", {}, {})})
{
}
size_t Code::CodeBuilder::addType(Engine::Runnable::TypeInfo const &type)
{
    m_types.push_back(type);
    return m_types.size() - 1;
}

std::optional<size_t> Code::CodeBuilder::getTypeByName(std::string const &name)
{
    if (std::vector<Engine::Runnable::TypeInfo>::const_iterator it = std::find_if(m_types.begin(), m_types.end(), [name](Engine::Runnable::TypeInfo const &t)
                                                                                  { return t.getName() == name; });
        it != m_types.end())
    {
        return it - m_types.begin();
    }
    return {};
}

size_t Code::CodeBuilder::getOrAddStringId(std::string const &str)
{
    if (std::vector<std::string>::const_iterator it = std::find(m_strings.begin(), m_strings.end(), str); it != m_strings.end())
    {
        return it - m_strings.begin();
    }
    m_strings.push_back(str);
    return m_strings.size() - 1;
}

Engine::Runnable::RunnableCode Code::CodeBuilder::getRunnableCode() const
{
    return Engine::Runnable::RunnableCode{
        .debugInfo = Debug::DebugInfo(m_functionDebugInfo),
        .functions = m_functions,
        .types = m_types,
        .strings = m_strings,
        .typeDeclarationLocations = m_typeDeclarationLocations};
}

void Code::CodeBuilder::createBlock()
{
    m_blocks.push_back(CodeBlock());
}

void Code::CodeBuilder::popBlock()
{
    m_blocks.pop_back();
}

void Code::CodeBuilder::addFunction(std::string const &name, Engine::Runnable::RunnableFunction func)
{
    m_functions[name] = func;
}

Code::Debug::FunctionDebugInfo &Code::CodeBuilder::getOrCreateDebugEntryForFunction(size_t typeId, std::string const &functionName)
{
    std::vector<Debug::FunctionDebugInfo>::iterator it = std::find_if(
        m_functionDebugInfo.begin(),
        m_functionDebugInfo.end(),
        [typeId, functionName](Debug::FunctionDebugInfo const &fd)
        { return fd.getTypeId() == typeId && fd.getName() == functionName; });
    if (it == m_functionDebugInfo.end())
    {
        m_functionDebugInfo.push_back(Debug::FunctionDebugInfo(typeId, functionName, ""));
        return m_functionDebugInfo.back();
    }
    return *it;
}

void Code::CodeBuilder::addTypeDeclarationLocation(std::string const &name, Debug::DebugInfoSourceData const &data)
{
    m_typeDeclarationLocations[name] = data;
}

void Code::CodeBlock::insert(std::vector<uint8_t> const &bytes)
{
    m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end());
}

void Code::CodeBlock::reserveBytesForJump()
{
    for (size_t i = 0; i < sizeof(Engine::JumpDistanceType); i++)
    {
        m_bytes.push_back(0);
    }
}

void Code::CodeBlock::addLabelPosition(std::string const &name, size_t pos)
{
    m_jumpLabelLocations[name] = pos;
}

void Code::CodeBlock::applyLabels()
{
    for (std::pair<std::string, std::vector<CodeJumpInfo>> jumps : m_jumpLabelDestinations)
    {
        // don't bother with checks or logic if there is nothing that even uses that label
        // TODO: Maybe issue a warning somehow?
        if (jumps.second.empty())
        {
            continue;
        }
        if (!m_jumpLabelLocations.contains(jumps.first))
        {
            throw Errors::ParsingError(jumps.second.front().row, jumps.second.front().column, std::string("Can not find label '" + jumps.first + "'"));
        }
        Engine::JumpDistanceType dest = (Engine::JumpDistanceType)m_jumpLabelLocations[jumps.first];
        for (CodeJumpInfo const &jump : jumps.second)
        {
            Engine::JumpDistanceType amount = dest - (Engine::JumpDistanceType)jump.byteCodeDestination;
            std::vector<uint8_t> addr = parseToBytes(dest - (Engine::JumpDistanceType)jump.byteCodeDestination);
            for (size_t i = 0; i < sizeof(Engine::JumpDistanceType); i++)
            {
                m_bytes[jump.byteCodeDestination + i] = addr[i];
            }
        }
    }
}

void Code::CodeBlock::addLabelDestination(std::string const &labelname, size_t jumpAddressBytesPosition, size_t codeColumn, size_t codeRow)
{
    if (!m_jumpLabelDestinations.contains(labelname))
    {
        m_jumpLabelDestinations[labelname] = {};
    }
    m_jumpLabelDestinations[labelname].push_back(CodeJumpInfo{.byteCodeDestination = jumpAddressBytesPosition, .column = codeColumn, .row = codeRow});
}
