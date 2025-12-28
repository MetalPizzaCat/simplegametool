#include "TypeManager.hpp"
#include "System/StandardLibrary.hpp"
#include "Object/AudioObject.hpp"
#include "System/Input.hpp"
#include <algorithm>

Engine::TypeManager::TypeManager()
{
    // load default types at the start so that they could be accessed by both runtime and compile time code
    addType(std::make_unique<ObjectType>("std",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(),                         // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>{{"pi", std::numbers::pi}}, // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),                          // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{{"sqrt", Standard::sqrt}}));

    addType(std::make_unique<ObjectType>("Input",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>{}, // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{{"is_key_pressed", Standard::Input::isKeyPressed}}));

    addType(std::make_unique<ObjectType>("Scancode",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         Input::Scancodes,                                               // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>()));
    addType(std::make_unique<ObjectType>("MouseButton",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         Input::MouseButtons,                                            // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>()));

    addType(std::make_unique<ObjectType>("AudioPlayer",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                             {"play", Standard::Audio::audioPlayerPlay}}));

    addType(std::make_unique<ObjectType>("Audio",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                             {"set_listener_position", Standard::Audio::audioPlayerPlay}}));

    addType(std::make_unique<ObjectType>("Random",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                             {"seed", Standard::Random::seed},
                                             {"rand_range_int", Standard::Random::getRandomIntInRange},
                                             {"rand_range_float", Standard::Random::getRandomFloatInRange}}));

    addType(std::make_unique<ObjectType>("Label",
                                         nullptr,
                                         nullptr,
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                         std::unordered_map<std::string, Runnable::CodeConstantValue>(), // constants
                                         std::unordered_map<std::string, Runnable::RunnableFunction>(),  // methods
                                         std::unordered_map<std::string, std::function<void(Scene & scene)>>{
                                             {"set_text", Standard::Label::setText},
                                             {"get_text", Standard::Label::getText},
                                             {"set_text_size", Standard::Label::setFontSize}}));
}

Engine::ObjectType const *Engine::TypeManager::getType(std::string const &name) const
{
    if (auto it = std::find_if(m_types.begin(), m_types.end(), [name](std::unique_ptr<ObjectType> const &t)
                               { return t->getName() == name; });
        it != m_types.end())
    {
        return it->get();
    }
    return nullptr;
}

bool Engine::TypeManager::doesTypeWithNameExist(std::string const &name) const
{
    return std::find_if(m_types.begin(), m_types.end(), [name](std::unique_ptr<ObjectType> const &t)
                        { return t->getName() == name; }) != m_types.end();
}

std::optional<std::string> Engine::TypeManager::getTypeDeclarationFileName(std::string const &typeName) const
{
    if (!m_typeDeclarationSourceFiles.contains(typeName))
    {
        return {};
    }
    return m_typeDeclarationSourceFiles.at(typeName);
}

Engine::ObjectType const *Engine::TypeManager::createStaticType(
    std::string const &name,
    std::string const &sourceFile,
    std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
    std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
    std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods)
{
    if (doesTypeWithNameExist(name))
    {
        throw TypeError("Multiple type declaration for type '" + name + "'");
    }
    m_typeDeclarationSourceFiles[name] = sourceFile;
    m_types.push_back(std::make_unique<ObjectType>(name,
                                                   nullptr,
                                                   nullptr,
                                                   std::unordered_map<std::string, Runnable::CodeConstantValue>(), // fields
                                                   constants,                                                      // constants
                                                   methods,                                                        // methods
                                                   nativeMethods));
    return m_types.back().get();
}

Engine::ObjectType const *Engine::TypeManager::createType(
    std::string const &name,
    std::string const &sourceFile,
    SpriteFramesAsset const *sprite,
    ObjectType const *parentType,
    std::unordered_map<std::string, Runnable::CodeConstantValue> const &fields,
    std::unordered_map<std::string, Runnable::CodeConstantValue> const &constants,
    std::unordered_map<std::string, Runnable::RunnableFunction> const &methods,
    std::unordered_map<std::string, std::function<void(Scene &scene)>> const &nativeMethods,
    std::vector<std::string> const &strings)
{
    if (doesTypeWithNameExist(name))
    {
        throw TypeError("Multiple type declaration for type '" + name + "'");
    }
    m_typeDeclarationSourceFiles[name] = sourceFile;
    m_types.push_back(std::make_unique<ObjectType>(name,
                                                   sprite,
                                                   parentType,
                                                   fields,    // fields
                                                   constants, // constants
                                                   methods,   // methods
                                                   nativeMethods,
                                                   strings));
    return m_types.back().get();
}

void Engine::TypeManager::addType(std::unique_ptr<ObjectType> type)
{
    m_types.push_back(std::move(type));
}

const char *Engine::TypeError::what() const throw()
{
    return m_message.c_str();
}

Engine::TypeError::TypeError(std::string const &msg) : m_message(msg)
{
}
