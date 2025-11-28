#include "CodeBuilder.hpp"
#include <algorithm>
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
        .functions = m_functions,
        .types = m_types,
        .strings = m_strings};
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

void Code::CodeBlock::insert(std::vector<uint8_t> const &bytes)
{
    m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end());
}
