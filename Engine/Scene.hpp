#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <iostream>
#include "Value.hpp"
#include "Instructions.hpp"
#include "../Code/CodeBuilder.hpp"
#include "MemoryObject.hpp"

namespace Engine
{

    struct Function
    {
        size_t start;
        size_t argumentCount;
    };
    /// @brief Scene represents a collection of objects with it's own script, which loads requires assets and types for preset object
    class Scene
    {
    public:
        explicit Scene(Runnable::RunnableCode const &code);

        void update(float delta);

        void draw(sf::RenderWindow &window)
        {
            for (std::unique_ptr<GameObject> const &obj : m_objects)
            {
                obj->draw(window);
            }
        }

        /// @brief Run function with a given name until the function ends
        /// @param name Name of the function
        void runFunctionByName(std::string const &name);

        /// @brief Run function from provided bytecode data
        /// @param func Function data
        void runFunction(Runnable::RunnableFunction const& func);

        void addType(std::string const &name, std::unique_ptr<ObjectType> type)
        {
            m_types.push_back(std::move(type));
            m_typeNames[name] = m_types.size() - 1;
        }

        void addFunction(std::string const &name, Runnable::RunnableFunction const &function)
        {
            m_functions[name] = function;
        }

        bool hasFunction(std::string const &name) const { return m_functions.contains(name); }

        StringObject *createString(std::string const &str);

        /// @brief Parse next `sizeof(T)` bytes into a T value using bitshifts and reinterpret cast
        /// @tparam T Type of the value to convert into
        /// @param start Where in the byte code to start from
        /// @return Parsed value
        template <typename T>
        T parseOperationConstant(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
        {
            uint64_t res = 0;
            for (uint64_t i = 0; i < sizeof(T) && (begin + i) != end; i++)
            {
                uint64_t offset = (sizeof(T) - i - 1) * 8;
                res |= (uint64_t)(*(begin + i)) << offset;
            }
            T *f = reinterpret_cast<T *>(&res);
            return *f;
        }

        std::optional<std::string> getConstantStringById(size_t id) const;

        Value popFromStackOrError();

    private:
        /// @brief Run nested function from scene method collection using provided callstack
        /// @param name Name of the function to run
        /// @param callStack Callstack to use for further nested calls
        void runNestedFunctionByName(std::string const &name, std::vector<size_t> &callStack);

        /// @brief Run nested function from provided bytecode using provided callstack
        /// @param func Function data to run
        /// @param callStack Callstack to use for further nested calls
        void runNestedFunction(Runnable::RunnableFunction const &func, std::vector<size_t> &callStack);

        std::vector<std::vector<Value>> m_operationStack = {{}};
        std::vector<Value> m_variables;
        std::vector<std::string> m_strings;
        std::map<std::string, size_t> m_typeNames;
        std::vector<std::unique_ptr<ObjectType>> m_types;
        std::map<std::string, Runnable::RunnableFunction> m_functions;
        /// @brief Various game objects that have various game logic
        std::vector<std::unique_ptr<GameObject>> m_objects;
        /// @brief List of all memory tracked objects such as strings and arrays
        std::vector<std::unique_ptr<MemoryObject>> m_memory;
    };
}