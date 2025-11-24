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
        void update()
        {
            if (hasFunction("update"))
            {
                runFunction("update");
            }
        }

        void draw(sf::RenderWindow &window)
        {
            for (std::unique_ptr<GameObject> const &obj : m_objects)
            {
                obj->draw(window);
            }
        }

        void runFunction(std::string const &name)
        {
            if (!m_functions.contains(name))
            {
                throw std::runtime_error("no function with given name found");
            }
            std::vector<size_t> callStack;
            runNestedFunction(name, callStack);
            std::cout << "Finished running " << name << std::endl;
        }

        void addType(std::string const &name, std::unique_ptr<ObjectType> type)
        {
            m_types.push_back(std::move(type));
            m_typeNames[name] = m_types.size() - 1;
        }

        void addFunction(std::string const &name, size_t pos, size_t argCount)
        {
            m_functions[name] = Function{.start = pos, .argumentCount = argCount};
        }

        bool hasFunction(std::string const &name) const { return m_functions.contains(name); }

        std::vector<uint8_t> &getBytes() { return m_byteCode; }

    private:
        void runNestedFunction(std::string const &name, std::vector<size_t> &callStack);
        
        std::vector<Value> m_variables;
        std::vector<std::string> m_strings;
        std::map<std::string, size_t> m_typeNames;
        std::vector<std::unique_ptr<ObjectType>> m_types;
        std::vector<uint8_t> m_byteCode;
        std::map<std::string, Function> m_functions;
        std::vector<std::unique_ptr<GameObject>> m_objects;
    };
}