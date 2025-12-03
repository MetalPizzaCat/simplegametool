#include "Scene.hpp"
#include "Error.hpp"
#include "StandardLibrary.hpp"

Engine::Scene::Scene(Runnable::RunnableCode const &code) : m_strings(code.strings), m_functions(code.functions), m_debugInfo(code.debugInfo)
{
    // special type that will be used to represent standard library functions and constants
    // should always exist as type 0, for sake of simplicity
    addType("std", std::make_unique<ObjectType>(nullptr,
                                                nullptr,
                                                std::map<std::string, Runnable::CodeConstantValue>(),
                                                std::map<std::string, Runnable::RunnableFunction>(),
                                                std::map<std::string, std::function<void(Scene & scene)>>{{"sqrt", Standard::sqrt}}));
    for (Runnable::TypeInfo const &type : code.types)
    {
        if (type.getName() == "std")
        {
            continue;
        }
        addType(type.getName(), std::make_unique<ObjectType>(ContentManager::getInstance().getAsset(type.getSpriteName()), type.getFields(), type.getMethods()));
    }
}

void Engine::Scene::update(float delta)
{
    for (auto const &obj : m_objects)
    {
        obj->update(delta);
        if (m_objects.back()->getType()->hasMethod("update"))
        {
            if (std::optional<size_t> typeId = getIdForType(obj->getType()); typeId.has_value())
            {
                runFunction(m_objects.back()->getType()->getMethod("update"), Runnable::RunnableFunctionDebugInfo{.typeId = typeId.value(), .functionName = "update"});
            }
            else
            {
                runFunction(m_objects.back()->getType()->getMethod("update"), {});
            }
        }
    }
    if (hasFunction("update"))
    {
        runFunctionByName("update");
    }
}

Engine::StringObject *Engine::Scene::createString(std::string const &str)
{
    m_memory.push_back(std::make_unique<StringObject>(str));
    return (Engine::StringObject *)m_memory.back().get();
}

std::optional<std::string> Engine::Scene::getConstantStringById(size_t id) const
{
    if (id >= m_strings.size())
    {
        return {};
    }
    return m_strings[id];
}

std::optional<std::string> Engine::Scene::getTypeNameById(size_t id) const
{
    std::map<std::string, size_t>::const_iterator it = std::find_if(m_typeNames.begin(), m_typeNames.end(), [id](auto const &t)
                                                                    { return t.second == id; });

    return it == m_typeNames.end() ? std::optional<std::string>{} : it->first;
}

Engine::Value Engine::Scene::popFromStackOrError()
{
    if (m_operationStack.empty() || m_operationStack.back().empty())
    {
        throw Errors::RuntimeMemoryError("Can not pop from stack because stack is empty");
    }
    Value r = m_operationStack.back().back();
    m_operationStack.back().pop_back();
    return r;
}

void Engine::Scene::setVariableValue(size_t id, Value const &val)
{
    if (m_variables.empty())
    {
        throw Errors::RuntimeMemoryError("No variable block is present");
    }
    std::vector<Value> &frame = m_variables.back();
    if (id > frame.size())
    {
        frame.resize(id + 1);
    }
    // TODO: handle ref counting for assigning/overwriting values
    frame[id] = val;
}

std::optional<Engine::Value> Engine::Scene::getVariableValue(size_t id) const
{
    if (m_variables.empty() || id >= m_variables.size())
    {
        return {};
    }
    return m_variables.back().at(id);
}

void Engine::Scene::createVariableBlock()
{
    m_variables.push_back({});
}

void Engine::Scene::popVariableBlock()
{
    m_variables.pop_back();
}

void Engine::Scene::pushToStack(Value const &v)
{
    if (m_operationStack.empty())
    {
        throw Errors::RuntimeMemoryError("Can not push to stack because no stack frame is available");
    }
    m_operationStack.back().push_back(v);
}

std::optional<size_t> Engine::Scene::getIdForType(ObjectType const *type) const
{
    if (std::vector<std::unique_ptr<ObjectType>>::const_iterator it = std::find_if(m_types.begin(), m_types.end(), [type](std::unique_ptr<ObjectType> const &t)
                                                                                   { return t.get() == type; });
        it != m_types.end())
    {
        return it - m_types.begin();
    }
    return {};
}

void Engine::Scene::error(std::optional<Runnable::RunnableFunctionDebugInfo> const &location, size_t position, std::string const &message)
{
    if (!location.has_value())
    {
        // if we don't have debug info just throw error without location info
        throw Errors::ExecutionError(message);
    }
    if (std::optional<std::pair<size_t, size_t>> filePos = m_debugInfo.getFilePositionForByte(location.value().typeId, location.value().functionName, position))
    {
        throw Errors::RuntimeError(filePos.value().first, filePos.value().second, message);
    }
    else
    {
        throw Errors::ExecutionError(message);
    }
}

void Engine::Scene::runFunctionByName(std::string const &name)
{

    Runnable::RunnableFunction const &func = m_functions.at(name);
    runFunction(func, Runnable::RunnableFunctionDebugInfo((size_t)-1, name));
}

void Engine::Scene::runFunction(Runnable::RunnableFunction const &func, std::optional<Runnable::RunnableFunctionDebugInfo> const &debugInfo)
{
    size_t pos = 0;
    createVariableBlock();
    for (size_t i = 0; i < func.argumentCount; i++)
    {
        // approach copied from goblang because it worked
        // does mean that these could be overriden during execution
        // but i am fine with it because c lets you do it and it works fine
        setVariableValue(i, popFromStackOrError());
    }
    try
    {
        m_operationStack.push_back({});
        while (pos < func.bytes.size())
        {

            // before you lies a giant switch case
            // but before you raise an concern think about it
            // this switch case *is* the interpreter and simply having a switch case(which is most likely going to converted into a jump table during compilation)
            // *is* a more efficient way to handle executing instructions
            switch ((Instructions)func.bytes.at(pos))
            {
            case Instructions::None:
                break;
            case Instructions::LoadConstString:
            {
                size_t typeId = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                if (std::optional<std::string> str = getConstantStringById(typeId); str.has_value())
                {
                    m_operationStack.back().push_back(createString(str.value()));
                }
                else
                {
                    error(debugInfo, pos, std::string("Unable to find string at id " + std::to_string(typeId)));
                }
            }
            break;
            case Instructions::CreateInstance:
            {
                size_t typeId = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                m_objects.push_back(std::make_unique<GameObject>(m_types[typeId].get(), *this));
                if (m_objects.back()->getType()->hasMethod("init"))
                {
                    runFunction(m_objects.back()->getType()->getMethod("init"), Runnable::RunnableFunctionDebugInfo(typeId, "init"));
                }

                pushToStack(m_objects.back().get());
            }
            break;
            case Instructions::PushInt:
                pushToStack(parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end()));
                pos += sizeof(int64_t);
                break;
            case Instructions::PushFloat:
                pushToStack(parseOperationConstant<double>(func.bytes.begin() + (pos + 1), func.bytes.end()));
                pos += sizeof(double);
                break;

            case Instructions::SetLocal:
            {
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                if (id >= m_variables.size())
                {
                    m_variables.resize(id + 1);
                }
                setVariableValue(id, m_operationStack.back().back());
                m_operationStack.back().pop_back();
            }
            break;
            case Instructions::GetLocal:
            {
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                if (std::optional<Value> var = getVariableValue(id); var.has_value())
                {
                    pushToStack(var.value());
                }
                else
                {
                    error(debugInfo, pos, "No variable with id '" + std::to_string(id) + "'is present in current context");
                }
            }
            break;
            case Instructions::Add:
            {
                Value a = m_operationStack.back().back();
                m_operationStack.back().pop_back();
                Value b = m_operationStack.back().back();
                m_operationStack.back().pop_back();
                if (a.index() != b.index())
                {
                    error(debugInfo, pos, "Attempted to perform arithmetic on incompatible types");
                }
                if (a.index() == ValueType::Vector)
                {
                    pushToStack(std::get<sf::Vector2f>(a) + std::get<sf::Vector2f>(b));
                }
                else if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) + std::get<int64_t>(b));
                }
            }
            break;
            case Instructions::Sub:
                break;
            case Instructions::Div:
                break;
            case Instructions::Mul:
                break;
            case Instructions::And:
                break;
            case Instructions::Or:
                break;
                break;
            case Instructions::SetPosition:
            {
                Value pos = m_operationStack.back().back();
                m_operationStack.back().pop_back();
                Value obj = m_operationStack.back().back();
                m_operationStack.back().pop_back();

                std::get<GameObject *>(obj)->setPosition(sf::Vector2f(std::get<sf::Vector2f>(pos)));
            }
            break;
            case Instructions::GetPosition:
            {
                Value obj = m_operationStack.back().back();
                m_operationStack.back().pop_back();

                pushToStack(std::get<GameObject *>(obj)->getPosition());
            }
            break;
            case Instructions::MakeVector:
            {
                if (m_operationStack.back().size() < 2)
                {
                    throw std::runtime_error("Stack doesn't have enough values for operation, expected 2");
                }
                Value a = m_operationStack.back().back();
                m_operationStack.back().pop_back();
                Value b = m_operationStack.back().back();
                m_operationStack.back().pop_back();
                pushToStack(sf::Vector2f(std::get<double>(a), std::get<double>(b)));
            }
            break;
            case Instructions::GetVectorX:
                break;
            case Instructions::GetVectorY:
                break;
            case Instructions::Print:
            {
                Value v = popFromStackOrError();
                std::cout << valueToString(v) << std::endl;
            }
            break;
            case Instructions::JumpBy:
            {
                JumpDistanceType dist = parseOperationConstant<JumpDistanceType>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += dist;
            }
            break;
            case Instructions::JumpByIf:
            {
                Value a = popFromStackOrError();
                if (a.index() != ValueType::Bool)
                {
                    error(debugInfo, pos, "Expected boolean value on stack for condition");
                }
                if (std::get<bool>(a))
                {
                    JumpDistanceType dist = parseOperationConstant<JumpDistanceType>(func.bytes.begin() + (pos + 1), func.bytes.end());
                    pos += dist;
                }
                else
                {
                    pos += sizeof(JumpDistanceType);
                }
            }
            break;
            case Instructions::Equals:
                break;
            case Instructions::NotEquals:
                break;
            case Instructions::More:
                break;
            case Instructions::Less:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();
                if (a.index() != b.index())
                {
                    error(debugInfo, pos, "Attempted to perform comparison on two different types");
                }
                if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) < std::get<int64_t>(b));
                }
                else
                {
                    error(debugInfo, pos, "Attempted to perform comparison on incompatible types");
                }
            }
            break;
            case Instructions::MoreOrEquals:
                break;
            case Instructions::LessOrEquals:
                break;
            case Instructions::CallMethod:
            {
                size_t typeId = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + pos + 1, func.bytes.end());
                pos += sizeof(size_t);
                if (typeId >= m_types.size())
                {
                    error(debugInfo, pos, "Invalid typeid. No type with id '" + std::to_string(typeId) + "' present");
                }
                if (std::optional<std::string> nameStr = getConstantStringById(id); nameStr.has_value())
                {
                    // yes the way errors are handled is awkward and tbh rather strange but this was the easier way to handle converting byte code to position
                    if (m_types[typeId]->isNativeMethod(nameStr.value()))
                    {
                        m_types[typeId]->callNativeMethod(nameStr.value(), *this);
                    }
                    else if (m_types[typeId]->hasMethod(nameStr.value()))
                    {
                        runFunction(m_types[typeId]->getMethod(nameStr.value()), Runnable::RunnableFunctionDebugInfo(typeId, nameStr.value()));
                    }

                    else
                    {

                        error(debugInfo, pos, "No method with name '" + nameStr.value() + "' in type '" + getTypeNameById(typeId).value() + "'");
                    }
                }
                else
                {
                    error(debugInfo, pos, "Invalid string constant id");
                }
            }
            break;
            // Call function of a scene
            case Instructions::CallFunction:
            {
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                if (std::optional<std::string> nameStr = getConstantStringById(id); nameStr.has_value())
                {
                    runFunctionByName(nameStr.value());
                }
                else
                {
                    error(debugInfo, pos, "Invalid string constant id");
                }
            }
            break;
            // Exit function without returning a value
            case Instructions::ExitFunction:
            {
                break;
            }
            break;
            // Return value from a function and  exit
            case Instructions::Return:
            {
                // TODO: add returning
                break;
            }
            default:
                throw Errors::InvalidInstructionError(std::string("Unknown instruction with value ") + std::to_string(func.bytes.at(pos)), pos);
            }

            pos++;
        }
    }
    // a bit awkward to catch exception just to throw it again
    // but this way we can avoid passing function data *everywhere*
    // and we can keep those functions free to use in non runnable code
    catch (Errors::RuntimeMemoryError e)
    {
        error(debugInfo, pos, e.what());
    }
    // TODO: do garbage collection here
    popVariableBlock();
}
