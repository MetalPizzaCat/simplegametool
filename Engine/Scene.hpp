#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Object/GameObject.hpp"
#include <iostream>
#include "Execution/Value.hpp"
#include "Execution/Instructions.hpp"
#include "../Code/CodeBuilder.hpp"
#include "Object/MemoryObject.hpp"
#include "Error.hpp"

namespace Engine
{

    /// @brief Scene represents a collection of objects with it's own script, which loads requires assets and types for preset object
    class Scene
    {
    public:
        /// @brief Create a new instance of scene object based on runnable code data
        /// @param code Code data to create from
        explicit Scene(Runnable::RunnableCode const &code);

        /// @brief Update all objects present in the scene and run their user defined update functions
        /// @param delta
        void update(float delta);

        void draw(sf::RenderWindow &window);

        /// @brief Call handlers for given event in scene and object scripts if handlers are present
        /// @param eventName Name of the event function(e.g. on_key_down, on_mouse_move)
        /// @param arguments Arguments to pass to functions
        void callSceneAndObjectScriptFunctionHandlers(std::string const &eventName, std::vector<Value> const &arguments);

        /// @brief Call handlers for key down even in scene and object scripts
        /// @param scancode Key scancode
        void handleKeyboardPress(sf::Keyboard::Scancode scancode);

        /// @brief Call handlers for mouse move event in scene and object scripts
        /// @param position Position relative to top left corner converted to floating point values
        void handleMouseMove(sf::Vector2f position);

        /// @brief Run function with a given name until the function ends
        /// @param name Name of the function
        void runFunctionByName(std::string const &name);

        /// @brief Run function from provided bytecode data
        /// @param func Function data
        /// @param debugInfo Optional information that is used for figuring out code location in case of an error
        void runFunction(Runnable::RunnableFunction const &func, std::optional<Runnable::RunnableFunctionDebugInfo> const &debugInfo);

        /// @brief Run method of a provided object passing any debug info that is available
        /// @param instance Instance to run the method from
        /// @param methodName Name of the method to run
        inline void runMethod(GameObject *instance, std::string const &methodName)
        {
            // methods should all technically expect self as first argument
            pushToStack(instance);
            if (std::optional<size_t> typeId = getIdForType(instance->getType()); typeId.has_value())
            {
                runFunction(instance->getType()->getMethod(methodName), Runnable::RunnableFunctionDebugInfo{.typeId = typeId.value(), .functionName = methodName});
            }
            else
            {
                runFunction(instance->getType()->getMethod(methodName), {});
            }
        }

        /// @brief Simple helper function that wraps the parsing and resolution process. If string is not found an error is thrown
        /// @param pos Position in the bytecode of the function
        /// @param byteCode Bytecode of the function
        /// @return Reference to the string in constant pool
        inline std::string const &parseByteOperandToString(size_t &pos, std::vector<uint8_t> const &byteCode)
        {
            size_t id = parseOperationConstant<size_t>(byteCode.begin() + pos + 1, byteCode.end());
            pos += sizeof(size_t);
            return getConstantStringById(id);
        }

        void addType(std::string const &name, std::unique_ptr<ObjectType> type)
        {
            m_types.push_back(std::move(type));
            m_typeNames[name] = m_types.size() - 1;
        }

        void addFunction(std::string const &name, Runnable::RunnableFunction const &function)
        {
            m_functions[name] = function;
        }

        /// @brief Check if given instance of scene has a function with given name
        /// @param name Function name
        /// @return
        bool hasFunction(std::string const &name) const { return m_functions.contains(name); }

        /// @brief Create a new string object and store it in the memory list
        /// @param str String to create the object from
        /// @return Pointer to the managed string object
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

        /// @brief Attempt to get string constant by id and throw memory error if no string uses given id
        /// @param id Id of the string constant
        /// @return String value
        std::string const &getConstantStringById(size_t id) const;

        std::optional<std::string> getTypeNameById(size_t id) const;

        /// @brief Pop value from current stack frame or throw error if no stack frame exists or stack is empty
        Value popFromStackOrError();

        /// @brief Attempt to get value from top of the current stack frame as given type. Throws error if no value is present or
        /// @tparam T
        /// @param errorMessage
        /// @return
        template <class T>
        T popFromStackAsType(std::string const &errorMessage)
        {
            if (m_operationStack.empty())
            {
                throw Errors::RuntimeMemoryError("Can not pop from stack because stack is empty");
            }
            if (!std::holds_alternative<T>(m_operationStack.back().back()))
            {
                throw Errors::RuntimeMemoryError(errorMessage);
            }
            Value v = m_operationStack.back().back();
            m_operationStack.back().pop_back();
            return std::get<T>(v);
        }

        /// @brief Set value of the variable in the current block
        /// @param id Id of the variable(block will be resized to fit)
        /// @param val Value to assign
        void setVariableValue(size_t id, Value const &val);

        /// @brief Get value of variable with given id in current block
        /// @param id Id of the variable
        /// @return Value or None if no block or variable is present
        std::optional<Value> getVariableValue(size_t id) const;

        void createVariableBlock();

        void popVariableBlock();

        /// @brief Push value onto the currect stack, or throw error if no stack exists
        /// @param v Value to push
        void pushToStack(Value const &v);

        /// @brief Push list of values onto the current stack or throw and error if no stack exists
        /// @param values Values to push
        void appendArrayToStack(std::vector<Value> const &values);

        std::optional<size_t> getIdForType(ObjectType const *type) const;

        /// @brief Throw an error with given info. Either throws an error with debug information or simple error depending on the availability of debug info
        /// @param location Location in the type list. Type id and function name are used to retrieve correct debug data
        /// @param position Position in th byte code of function that is running
        /// @param message Message to display in the exception
        void error(std::optional<Runnable::RunnableFunctionDebugInfo> const &location, size_t position, std::string const &message);

        GameObject *getObjectByName(std::string const &name) const;

        /// @brief Get "global" variable by name or throw error if no variable uses that name. "Global" variable is still local to the scene
        /// @param name Name of the variable
        /// @return Value of the variable
        Value getGlobalVariable(std::string const &name) const;

        /// @brief Set "global" variable by name or throw error if no variable uses that name. "Global" variable is still local to the scene
        /// @param name Name of the variable
        /// @param v Value to set
        inline void setGlobalVariable(std::string const &name, Value const &v)
        {
            m_globals[name] = v;
        }

        void changeScene(std::string const &targetScene);

        void endScene();

        bool isFinished() const { return m_quitting; }

        std::optional<std::string> getNextScene() const { return m_nextScene; }

        void collectGarbage();

    private:
        std::optional<std::string> m_nextScene;
        std::vector<std::vector<Value>> m_operationStack = {{}};
        std::vector<std::vector<Value>> m_variables;
        std::unordered_map<std::string, Value> m_globals;
        std::vector<std::string> m_strings;
        std::unordered_map<std::string, size_t> m_typeNames;
        std::vector<std::unique_ptr<ObjectType>> m_types;
        std::unordered_map<std::string, Runnable::RunnableFunction> m_functions;
        /// @brief Various game objects that have various game logic
        std::vector<std::unique_ptr<GameObject>> m_objects;
        /// @brief List of all memory tracked objects such as strings and arrays
        std::vector<std::unique_ptr<MemoryObject>> m_memory;

        Code::Debug::DebugInfo m_debugInfo;

        bool m_quitting = false;
    };
}