#include "ArrayObject.hpp"
#include "../Error.hpp"

Engine::ArrayObject::ArrayObject(size_t initialSize) : m_values(initialSize, NilValue)
{
}

Engine::ArrayObject::ArrayObject(std::vector<Value> const &values) : m_values(values)
{
}

std::string Engine::ArrayObject::toString() const
{
    std::string result = "[";
    for (size_t i = 0; i < m_values.size(); i++)
    {
        result += valueToString(m_values[i]);
        if (i != m_values.size() - 1)
        {
            result += ',';
        }
    }
    return result + "]";
}

void Engine::ArrayObject::setItem(size_t id, Value const &v)
{
    if (id >= m_values.size())
    {
        return;
    }
    decreaseValueRefCount(m_values[id]);
    m_values[id] = v;
    increaseValueRefCount(v);
}

Engine::Value Engine::ArrayObject::getItem(size_t id) const
{
    if (id >= m_values.size())
    {
        throw Errors::RuntimeMemoryError(std::string("Attempted to get item at position " + std::to_string(id) + " in array of size " + std::to_string(m_values.size())));
    }
    return m_values.at(id);
}

void Engine::ArrayObject::appendItem(Value const &v)
{
    m_values.push_back(v);
}

Engine::ArrayObject::~ArrayObject()
{
    for (Value const &v : m_values)
    {
        decreaseValueRefCount(v);
    }
}
