#include "CodeBuilder.hpp"
#include <algorithm>
#include "../Engine/Execution/Instructions.hpp"
#include "Error.hpp"
Code::CodeBuilder::CodeBuilder()
{
}
size_t Code::CodeBuilder::getOrAddStringId(std::string const &str)
{
    if (m_strings.empty())
    {
        // the first block is used as the "global" block which is used for scene code parsing
        createStringBlock();
    }
    if (std::vector<std::string>::const_iterator it = std::find(m_strings.back().begin(), m_strings.back().end(), str); it != m_strings.back().end())
    {
        return it - m_strings.back().begin();
    }
    m_strings.back().push_back(str);
    return m_strings.back().size() - 1;
}

std::vector<std::string> Code::CodeBuilder::popStringBlock()
{
    if (m_strings.empty())
    {
        return {};
    }
    std::vector<std::string> r = m_strings.back();
    m_strings.pop_back();
    return r;
}
Engine::Runnable::RunnableCode Code::CodeBuilder::getRunnableCode() const
{
    return Engine::Runnable::RunnableCode{
        .debugInfo = Debug::DebugInfo(m_functionDebugInfo),
        .functions = m_functions,
        .strings = m_strings.empty() ? std::vector<std::string>() : m_strings.front(),
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

Code::Debug::FunctionDebugInfo &Code::CodeBuilder::getOrCreateDebugEntryForFunction(std::string const& typeName, std::string const &functionName)
{
    std::vector<Debug::FunctionDebugInfo>::iterator it = std::find_if(
        m_functionDebugInfo.begin(),
        m_functionDebugInfo.end(),
        [typeName, functionName](Debug::FunctionDebugInfo const &fd)
        { return fd.getTypeName() == typeName && fd.getName() == functionName; });
    if (it == m_functionDebugInfo.end())
    {
        m_functionDebugInfo.push_back(Debug::FunctionDebugInfo(typeName, functionName, ""));
        return m_functionDebugInfo.back();
    }
    return *it;
}

bool Code::CodeBuilder::addTypeDeclarationLocation(std::string const &name, Debug::DebugInfoSourceData const &data)
{
    if (m_typeDeclarationLocations.contains(name))
    {
        return false;
    }
    m_typeDeclarationLocations[name] = data;
    return true;
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

size_t Code::CodeBlock::addVariableName(std::string const &name)
{
    m_variables.push_back(name);
    return m_variables.size();
}

std::optional<size_t> Code::CodeBlock::getVariableId(std::string const &name) const
{
    if (std::vector<std::string>::const_iterator it = std::find(m_variables.begin(), m_variables.end(), name);
        it != m_variables.end())
    {
        return it - m_variables.begin();
    }
    return {};
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
