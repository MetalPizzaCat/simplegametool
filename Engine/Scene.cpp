#include "Scene.hpp"
#include "Error.hpp"
#include "StandardLibrary.hpp"
#include "AudioObject.hpp"
#include "Input.hpp"
#include "Random.hpp"

#include <numbers>

Engine::Scene::Scene(Runnable::RunnableCode const &code) : m_strings(code.strings), m_functions(code.functions), m_debugInfo(code.debugInfo)
{
    // special type that will be used to represent standard library functions and constants
    // should always exist as type 0, for sake of simplicity, every other "default" type will be added this way
    addType("std", std::make_unique<ObjectType>(nullptr,
                                                nullptr,
                                                std::unordered_map<std::string, Runnable::CodeConstantValue>(),                         // fields
                                                std::unordered_map<std::string, Runnable::CodeConstantValue>{{"pi", std::numbers::pi}}, // constants
                                                std::unordered_map<std::string, Runnable::RunnableFunction>(),                          // methods
                                                std::unordered_map<std::string, std::function<void(Scene & scene)>>{{"sqrt", Standard::sqrt}}));

    addType("Input", std::make_unique<ObjectType>(nullptr,
                                                  nullptr,
                                                  std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                  std::unordered_map<std::string, Runnable::CodeConstantValue>{}, // constants
                                                  std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                  std::unordered_map<std::string, std::function<void(Scene & scene)>>{{"is_key_pressed", Standard::Input::isKeyPressed}}));

    addType("Scancode", std::make_unique<ObjectType>(nullptr,
                                                     nullptr,
                                                     std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                     Input::Scancodes,                                               // constants
                                                     std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                     std::unordered_map<std::string, std::function<void(Scene & scene)>>()));
    addType("MouseButton", std::make_unique<ObjectType>(nullptr,
                                                        nullptr,
                                                        std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                        Input::MouseButtons,                                            // constants
                                                        std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                        std::unordered_map<std::string, std::function<void(Scene & scene)>>()));

    addType("AudioPlayer", std::make_unique<ObjectType>(nullptr,
                                                        nullptr,
                                                        std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                        std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                                        std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                        std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                                            {"play", Standard::Audio::audioPlayerPlay}}));

    addType("Audio", std::make_unique<ObjectType>(nullptr,
                                                  nullptr,
                                                  std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                  std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                                  std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                  std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                                      {"set_listener_position", Standard::Audio::audioPlayerPlay}}));

    addType("Random", std::make_unique<ObjectType>(nullptr,
                                                   nullptr,
                                                   std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                   std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                                   std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                                   std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                                       {"seed", Standard::Random::seed},
                                                       {"rand_range_int", Standard::Random::getRandomIntInRange},
                                                       {"rand_range_float", Standard::Random::getRandomFloatInRange}}));
    for (Runnable::TypeInfo const &type : code.types)
    {
        if (type.isDefaultType())
        {
            continue;
        }
        SpriteFramesAsset const *asset = ContentManager::getInstance().getAnimationAsset(type.getSpriteName());
        if (asset == nullptr)
        {
            if (code.typeDeclarationLocations.contains(type.getName()))
            {
                throw Errors::RuntimeError(
                    code.typeDeclarationLocations.at(type.getName()).row,
                    code.typeDeclarationLocations.at(type.getName()).column,
                    "Type '" + type.getName() + "' uses nonexistant asset '" + type.getSpriteName() + "'");
            }
            else
            {
                throw Errors::ExecutionError("Type '" + type.getName() + "' uses nonexistant asset '" + type.getSpriteName() + "'");
            }
        }
        addType(type.getName(), std::make_unique<ObjectType>(asset, type.getFields(), type.getConstants(), type.getMethods()));
    }
}

void Engine::Scene::update(float delta)
{
    for (auto const &obj : m_objects)
    {
        obj->update(delta);
        if (obj->getType()->hasMethod("update"))
        {
            runMethod(obj.get(), "update");
        }
        if (obj->hasAnimationJustFinished())
        {
            obj->setAnimationJustFinished(false);
            if (obj->getType()->hasMethod("on_animation_ended"))
            {
                runMethod(obj.get(), "on_animation_ended");
            }
        }
    }
    if (hasFunction("update"))
    {
        runFunctionByName("update");
    }
}

void Engine::Scene::draw(sf::RenderWindow &window)
{
    for (std::unique_ptr<GameObject> const &obj : m_objects)
    {
        obj->draw(window);
    }
}
void Engine::Scene::callSceneAndObjectScriptFunctionHandlers(std::string const &eventName, std::vector<Value> const &arguments)
{
    for (auto const &obj : m_objects)
    {
        if (obj->getType()->hasMethod(eventName))
        {
            appendArrayToStack(arguments);
            runMethod(obj.get(), eventName);
        }
    }
    if (hasFunction(eventName))
    {
        appendArrayToStack(arguments);
        runFunctionByName(eventName);
    }
}
void Engine::Scene::handleKeyboardPress(sf::Keyboard::Scancode scancode)
{
    callSceneAndObjectScriptFunctionHandlers("on_key_down", {(int64_t)scancode});
}
void Engine::Scene::handleMouseMove(sf::Vector2f position)
{
    callSceneAndObjectScriptFunctionHandlers("on_mouse_move", {position});
}

Engine::StringObject *Engine::Scene::createString(std::string const &str)
{
    m_memory.push_back(std::make_unique<StringObject>(str));
    return (Engine::StringObject *)m_memory.back().get();
}

std::string const &Engine::Scene::getConstantStringById(size_t id) const
{
    if (id >= m_strings.size())
    {
        throw Errors::RuntimeMemoryError("Unable to find string constant with id " + std::to_string(id));
    }
    return m_strings[id];
}

std::optional<std::string> Engine::Scene::getTypeNameById(size_t id) const
{
    std::unordered_map<std::string, size_t>::const_iterator it = std::find_if(m_typeNames.begin(), m_typeNames.end(), [id](auto const &t)
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
    if (id >= frame.size())
    {
        frame.resize(id + 1);
    }
    // TODO: handle ref counting for assigning/overwriting values
    frame[id] = val;
}

std::optional<Engine::Value> Engine::Scene::getVariableValue(size_t id) const
{
    if (m_variables.empty() || id >= m_variables.back().size())
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

void Engine::Scene::appendArrayToStack(std::vector<Value> const &values)
{
    if (m_operationStack.empty())
    {
        throw Errors::RuntimeMemoryError("Can not push to stack because no stack frame is available");
    }
    m_operationStack.back().insert(m_operationStack.back().end(), values.begin(), values.end());
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

Engine::GameObject *Engine::Scene::getObjectByName(std::string const &name) const
{
    if (std::vector<std::unique_ptr<GameObject>>::const_iterator it = std::find_if(m_objects.begin(), m_objects.end(), [name](std::unique_ptr<GameObject> const &o)
                                                                                   { return o->getName() == name; });
        it != m_objects.end())
    {
        return it->get();
    }

    return nullptr;
}

void Engine::Scene::collectGarbage()
{
    for (int64_t i = m_memory.size() - 1; i >= 0; i--)
    {
        if (m_memory[i]->isDead())
        {
            m_memory.erase(m_memory.begin() + i);
        }
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
        bool returning = false;
        while (pos < func.bytes.size() && !returning)
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
                m_operationStack.back().push_back(createString(getConstantStringById(typeId)));
            }
            break;
            case Instructions::CreateInstance:
            {
                std::string const &name = popFromStackAsType<StringObject *>("Expected string for object name")->getString();
                size_t typeId = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                m_objects.push_back(std::make_unique<GameObject>(m_types[typeId].get(), name, *this));
                GameObject *inst = m_objects.back().get();
                if (m_objects.back()->getType()->hasMethod("init"))
                {
                    pushToStack(inst);
                    runFunction(m_objects.back()->getType()->getMethod("init"), Runnable::RunnableFunctionDebugInfo(typeId, "init"));
                }

                pushToStack(inst);
            }
            break;
            case Instructions::GetInstanceByName:
            {
                std::string const &name = popFromStackAsType<StringObject *>("Expected string for object name")->getString();
                if (GameObject *obj = getObjectByName(name); obj != nullptr)
                {
                    pushToStack(obj);
                }
                else
                {
                    error(debugInfo, pos, "No object named '" + name + "' found");
                }
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

            case Instructions::PushVector:
            {
                double x = parseOperationConstant<double>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(double);
                double y = parseOperationConstant<double>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(double);
                pushToStack(sf::Vector2f(x, y));
            }
            break;

            case Instructions::SetLocal:
            {
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                if (id >= m_variables.back().size())
                {
                    m_variables.back().resize(id + 1);
                }
                setVariableValue(id, popFromStackOrError());
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
                Value a = popFromStackOrError();
                Value b = popFromStackOrError();
                if (a.index() != b.index())
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
                if (a.index() == ValueType::Vector)
                {
                    pushToStack(std::get<sf::Vector2f>(a) + std::get<sf::Vector2f>(b));
                }
                else if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) + std::get<int64_t>(b));
                }
                else if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) + std::get<double>(b));
                }
            }
            break;
            case Instructions::Sub:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();

                if (a.index() != b.index())
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
                if (a.index() == ValueType::Vector)
                {
                    pushToStack(std::get<sf::Vector2f>(a) - std::get<sf::Vector2f>(b));
                }
                else if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) - std::get<int64_t>(b));
                }
                else if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) - std::get<double>(b));
                }
            }
            break;
            case Instructions::Div:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();

                if (a.index() == ValueType::Vector && b.index() == ValueType::Float)
                {
                    pushToStack(std::get<sf::Vector2f>(a) / (float)std::get<double>(b));
                }
                if (a.index() != b.index())
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
                else if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) / std::get<int64_t>(b));
                }
                else if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) / std::get<double>(b));
                }
                else
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
            }
            break;
            case Instructions::Mul:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();

                if (a.index() == ValueType::Vector && b.index() == ValueType::Float)
                {
                    pushToStack(std::get<sf::Vector2f>(a) * (float)std::get<double>(b));
                }
                if (a.index() != b.index())
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
                else if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) * std::get<int64_t>(b));
                }
                else if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) * std::get<double>(b));
                }
                else
                {
                    error(debugInfo, pos, std::string("Attempted to perform arithmetic on incompatible types: ") + typeToString((ValueType)a.index()) + " and " + typeToString((ValueType)b.index()));
                }
            }
            break;
            case Instructions::And:
                break;
            case Instructions::Or:
                break;
            case Instructions::Not:
            {
                pushToStack(!popFromStackAsType<bool>("Expected boolean value on stack"));
            }
            break;
            case Instructions::SetPosition:
            {
                sf::Vector2f objPos = popFromStackAsType<sf::Vector2f>("Expected vector for position on stack");
                GameObject *obj = popFromStackAsType<GameObject *>("Expected object to assign position for on stack");

                obj->setPosition(objPos);
            }
            break;
            case Instructions::GetPosition:
            {
                GameObject *obj = popFromStackAsType<GameObject *>("Expected object to get position from on stack");

                pushToStack(obj->getPosition());
            }
            break;
            case Instructions::MakeVector:
            {
                Value y = popFromStackAsType<double>("Expected float on stack for y");
                Value x = popFromStackAsType<double>("Expected float on stack for x");

                pushToStack(sf::Vector2f(std::get<double>(x), std::get<double>(y)));
            }
            break;
            case Instructions::GetVectorX:
                pushToStack(popFromStackAsType<sf::Vector2f>("Expected vector for position on stack").x);
                break;
            case Instructions::GetVectorY:
                pushToStack(popFromStackAsType<sf::Vector2f>("Expected vector for position on stack").y);
                break;
            case Instructions::Print:
            {
                std::cout << valueToString(popFromStackOrError()) << '\n';
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
                if (popFromStackAsType<bool>("Expected boolean value on stack for condition"))
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
            {
                Value a = popFromStackOrError();
                Value b = popFromStackOrError();
                if (a.index() != b.index())
                {
                    pushToStack(false);
                }
                switch (a.index())
                {
                case ValueType::Bool:
                    pushToStack(std::get<bool>(a) == std::get<bool>(b));
                    break;
                case ValueType::Int:
                    pushToStack(std::get<int64_t>(a) == std::get<int64_t>(b));
                    break;
                case ValueType::Float:
                    pushToStack(std::get<double>(a) == std::get<double>(b));
                    break;
                case ValueType::Vector:
                    pushToStack(std::get<sf::Vector2f>(a) == std::get<sf::Vector2f>(b));
                    break;
                case ValueType::Object:
                    pushToStack(std::get<GameObject *>(a) == std::get<GameObject *>(b));
                    break;
                case ValueType::String:
                    pushToStack(std::get<StringObject *>(a)->getString() == std::get<StringObject *>(b)->getString());
                    break;
                }
            }
            break;
            case Instructions::NotEquals:
            {
                Value a = popFromStackOrError();
                Value b = popFromStackOrError();
                if (a.index() != b.index())
                {
                    pushToStack(true);
                }
                switch (a.index())
                {
                case ValueType::Bool:
                    pushToStack(std::get<bool>(a) != std::get<bool>(b));
                    break;
                case ValueType::Int:
                    pushToStack(std::get<int64_t>(a) != std::get<int64_t>(b));
                    break;
                case ValueType::Float:
                    pushToStack(std::get<double>(a) != std::get<double>(b));
                    break;
                case ValueType::Vector:
                    pushToStack(std::get<sf::Vector2f>(a) != std::get<sf::Vector2f>(b));
                    break;
                case ValueType::Object:
                    pushToStack(std::get<GameObject *>(a) != std::get<GameObject *>(b));
                    break;
                case ValueType::String:
                    pushToStack(std::get<StringObject *>(a)->getString() != std::get<StringObject *>(b)->getString());
                    break;
                }
            }
            break;
            case Instructions::More:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();
                if (a.index() != b.index())
                {
                    error(debugInfo,
                          pos,
                          std::string("Attempted to perform comparison on two different types: ") +
                              typeToString((ValueType)a.index()) +
                              " and " +
                              typeToString((ValueType)b.index()));
                }
                if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) > std::get<int64_t>(b));
                }
                if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) > std::get<double>(b));
                }
                else
                {
                    error(debugInfo, pos, "Attempted to perform comparison on incompatible types");
                }
            }
            break;
            case Instructions::Less:
            {
                Value b = popFromStackOrError();
                Value a = popFromStackOrError();
                if (a.index() != b.index())
                {
                    error(debugInfo,
                          pos,
                          std::string("Attempted to perform comparison on two different types: ") +
                              typeToString((ValueType)a.index()) +
                              " and " +
                              typeToString((ValueType)b.index()));
                }
                if (a.index() == ValueType::Int)
                {
                    pushToStack(std::get<int64_t>(a) < std::get<int64_t>(b));
                }
                if (a.index() == ValueType::Float)
                {
                    pushToStack(std::get<double>(a) < std::get<double>(b));
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
                GameObject *obj = popFromStackAsType<GameObject *>("Expected object on stack to call method from");
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + pos + 1, func.bytes.end());
                pos += sizeof(size_t);
                std::string const &name = getConstantStringById(id);
                if (obj->getType()->isNativeMethod(name))
                {
                    pushToStack(obj);
                    obj->getType()->callNativeMethod(name, *this);
                }
                else if (obj->getType()->hasMethod(name))
                {
                    runMethod(obj, name);
                }
                else
                {
                    error(debugInfo, pos, "No method with name '" + name + "' in type '" + getTypeNameById(getIdForType(obj->getType()).value()).value() + "'");
                }
            }
            break;
            case Instructions::CallMethodStatic:
            {
                size_t typeId = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + pos + 1, func.bytes.end());
                pos += sizeof(size_t);
                if (typeId >= m_types.size())
                {
                    error(debugInfo, pos, "Invalid typeid. No type with id '" + std::to_string(typeId) + "' present");
                }
                std::string const &name = getConstantStringById(id);
                // yes the way errors are handled is awkward and tbh rather strange but this was the easier way to handle converting byte code to position
                if (m_types[typeId]->isNativeMethod(name))
                {
                    m_types[typeId]->callNativeMethod(name, *this);
                }
                else if (m_types[typeId]->hasMethod(name))
                {
                    runFunction(m_types[typeId]->getMethod(name), Runnable::RunnableFunctionDebugInfo(typeId, name));
                }
                else
                {

                    error(debugInfo, pos, "No method with name '" + name + "' in type '" + getTypeNameById(typeId).value() + "'");
                }
            }
            break;
            // Call function of a scene
            case Instructions::CallFunction:
            {
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                runFunctionByName(getConstantStringById(id));
            }
            break;
            // Exit function without returning a value
            case Instructions::ExitFunction:
            {
                returning = true;
                break;
            }
            break;
            // Return value from a function and  exit
            case Instructions::Return:
            {
                // TODO: add returning
                break;
            }
            case Instructions::GetField:
            {
                std::string const &fieldName = parseByteOperandToString(pos, func.bytes);
                GameObject *obj = popFromStackAsType<GameObject *>("Expected game object on stack");
                if (std::optional<Value> val = obj->getFieldValue(fieldName); val.has_value())
                {
                    pushToStack(val.value());
                }
                else
                {
                    error(debugInfo, pos, "No field named '" + fieldName + "' in object '" + obj->getName() + "'");
                }
            }
            break;
            case Instructions::SetField:
            {
                std::string const &fieldName = parseByteOperandToString(pos, func.bytes);
                Value v = popFromStackOrError();
                popFromStackAsType<GameObject *>("Expected game object on stack")->setFieldValue(fieldName, v);
            }
            break;
            case Instructions::HasField:
            {
                std::string const &fieldName = popFromStackAsType<StringObject *>("Expected field name on stack")->getString();
                pushToStack(popFromStackAsType<GameObject *>("Expected game object on stack")->hasField(fieldName));
            }
            break;
            case Instructions::GetConst:
            {
                size_t typeId = parseOperationConstant<size_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
                pos += sizeof(size_t);
                size_t id = parseOperationConstant<size_t>(func.bytes.begin() + pos + 1, func.bytes.end());
                pos += sizeof(size_t);
                if (typeId >= m_types.size())
                {
                    error(debugInfo, pos, "Invalid typeid. No type with id '" + std::to_string(typeId) + "' present");
                }
                if (std::optional<Value> v = m_types[typeId]->getConstant(getConstantStringById(id), *this); v.has_value())
                {
                    pushToStack(v.value());
                }
                else
                {
                    error(debugInfo, pos, "No constant with name '" + getConstantStringById(id) + "' in type '" + getTypeNameById(typeId).value() + "'");
                }
            }
            break;

            case Instructions::CreateSoundPlayer:
            {
                std::string const &assetName = popFromStackAsType<StringObject *>("Expected audio asset name")->getString();
                m_objects.push_back(std::make_unique<AudioObject>(m_types[m_typeNames["AudioPlayer"]].get(),
                                                                  assetName,
                                                                  popFromStackAsType<StringObject *>("Expected object name")->getString(), *this));
                pushToStack(m_objects.back().get());
            }
            break;
            case Instructions::PlaySound:
            {
            }
            break;
            case Instructions::GetSize:
            {
                GameObject *obj = popFromStackAsType<GameObject *>("Expected object on stack");
                pushToStack(obj->getSize());
            }
            break;
            case Instructions::SetSize:
            {
                sf::Vector2f size = popFromStackAsType<sf::Vector2f>("Expected size on stack");
                popFromStackAsType<GameObject *>("Expected object on stack")->setSize(size);
            }
            break;
            case Instructions::AreOverlapping:
            {
                GameObject *obj = popFromStackAsType<GameObject *>("Expected object on stack");
                GameObject *obj2 = popFromStackAsType<GameObject *>("Expected object on stack");
                pushToStack(sf::FloatRect(obj->getPosition(), obj->getSize()).findIntersection(sf::FloatRect(obj2->getPosition(), obj2->getSize())).has_value());
            }
            break;
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

    collectGarbage();
    m_operationStack.pop_back();
    popVariableBlock();
}
