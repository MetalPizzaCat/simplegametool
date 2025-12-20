#pragma once
#include "MemoryObject.hpp"
#include "../Execution/Value.hpp"
namespace Engine
{
    class ArrayObject : public MemoryObject
    {
    public:
        /// @brief Create array of given sized fill with Nil values
        /// @param initialSize
        explicit ArrayObject(size_t initialSize);
        explicit ArrayObject(std::vector<Value> const &values);
        std::string toString() const override;

        void setItem(size_t id, Value const &v);

        /// @brief Try to get item at given id and throw error if id is out of bounds
        /// @param id Index of the item
        /// @return Item at the given position
        Engine::Value getItem(size_t id) const;

        void appendItem(Value const &v);

        std::vector<Value> &getItems() { return m_values; }

        size_t getLength() const { return m_values.size(); }

        virtual ~ArrayObject();

    private:
        std::vector<Value> m_values;
    };
} // namespace Engine
