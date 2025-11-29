#include "Scene.hpp"
#include "Error.hpp"

Engine::Scene::Scene(Runnable::RunnableCode const &code) : m_strings(code.strings), m_functions(code.functions)
{
    for (Runnable::TypeInfo const &type : code.types)
    {
        addType(type.getName(), std::make_unique<ObjectType>(ContentManager::getInstance().getAsset(type.getSpriteName()), type.getFields()));
    }
}

void Engine::Scene::runFunction(std::string const &name)
{
    if (!m_functions.contains(name))
    {
        throw std::runtime_error("no function with given name found");
    }
    std::vector<size_t> callStack;
    runNestedFunction(name, callStack);
    std::cout << "Finished running " << name << std::endl;
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

void Engine::Scene::runNestedFunction(std::string const &name, std::vector<size_t> &callStack)
{
    Runnable::RunnableFunction const &func = m_functions.at(name);
    size_t pos = 0;
    m_operationStack.push_back({});
    while (pos < func.bytes.size())
    {
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
                throw Engine::Errors::RuntimeError(std::string("Unable to find string at id " + std::to_string(typeId)));
            }
        }
        break;
        case Instructions::CreateInstance:
        {
            size_t typeId = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
            pos += sizeof(size_t);
            m_objects.push_back(std::make_unique<GameObject>(m_types[typeId].get(), *this));
            m_operationStack.back().push_back(m_objects.back().get());
        }
        break;
        case Instructions::PushInt:
            m_operationStack.back().push_back(parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end()));
            pos += sizeof(int64_t);
            break;
        case Instructions::PushFloat:
            m_operationStack.back().push_back(parseOperationConstant<double>(func.bytes.begin() + (pos + 1), func.bytes.end()));
            pos += sizeof(double);
            break;

        case Instructions::SetLocal:
        {
            size_t id = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
            pos += sizeof(size_t);
            if (id >= m_variables.size())
            {
                m_variables.resize(id + 1);
            }
            m_variables[id] = m_operationStack.back().back();
            m_operationStack.back().pop_back();
        }
        break;
        case Instructions::GetLocal:
        {
            size_t id = parseOperationConstant<int64_t>(func.bytes.begin() + (pos + 1), func.bytes.end());
            pos += sizeof(size_t);
            m_operationStack.back().push_back(m_variables[id]);
        }
        break;
        case Instructions::Add:
        {
            Value a = m_operationStack.back().back();
            m_operationStack.back().pop_back();
            Value b = m_operationStack.back().back();
            m_operationStack.back().pop_back();
            if (a.index() == ValueType::Vector)
            {
                m_operationStack.back().push_back(std::get<sf::Vector2f>(a) + std::get<sf::Vector2f>(b));
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
        case Instructions::Call:
            break;
        case Instructions::Return:
        {
            if (callStack.empty())
            {
                return;
            }
            else
            {
                pos = callStack.back();
                callStack.pop_back();
            }
        }
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

            m_operationStack.back().push_back(std::get<GameObject *>(obj)->getPosition());
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
            m_operationStack.back().push_back(sf::Vector2f(std::get<double>(a), std::get<double>(b)));
        }
        break;
        case Instructions::GetVectorX:
            break;
        case Instructions::GetVectorY:
            break;
        }
        pos++;
    }
}