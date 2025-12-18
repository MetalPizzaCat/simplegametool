#pragma once
#include "MemoryObject.hpp"
#include "../Execution/Value.hpp"
namespace Engine
{
    class ArrayObject : public MemoryObject
    {
    public:
        std::string toString() const override;

    private:
        std::vector<Value> m_values;
    };
} // namespace Engine
