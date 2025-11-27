#include "Scene.hpp"

Engine::Scene::Scene(Runnable::RunnableCode const &code) : m_strings(code.strings)
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

void Engine::Scene::runNestedFunction(std::string const &name, std::vector<size_t> &callStack)
{
    size_t pos = m_functions[name].start;
    std::vector<Value> stack;
    while (pos < m_byteCode.size())
    {
        switch ((Instructions)m_byteCode[pos])
        {
        case Instructions::None:
            break;
        case Instructions::LoadConstString:
            break;
        case Instructions::LoadConstInt:
            break;
        case Instructions::LoadConstFloat:
            break;
        case Instructions::LoadType:
            break;
        case Instructions::CreateInstance:
        {
            size_t typeId = m_byteCode[++pos];
            m_objects.push_back(std::make_unique<GameObject>(m_types[typeId].get()));
            stack.push_back(m_objects.back().get());
        }
        break;
        case Instructions::PushInt:
            break;
        case Instructions::PushFloat:
            stack.push_back((float)m_byteCode[++pos]);
            break;
        case Instructions::SetLocal:
        {
            size_t id = m_byteCode[++pos];
            if (id >= m_variables.size())
            {
                m_variables.resize(id + 1);
            }
            m_variables[id] = stack.back();
            stack.pop_back();
        }
        break;
        case Instructions::GetLocal:
        {
            size_t id = m_byteCode[++pos];
            stack.push_back(m_variables[id]);
        }
        break;
        case Instructions::Add:
        {
            Value a = stack.back();
            stack.pop_back();
            Value b = stack.back();
            stack.pop_back();
            if (a.index() == ValueType::Vector)
            {
                stack.push_back(std::get<sf::Vector2f>(a) + std::get<sf::Vector2f>(b));
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
            Value pos = stack.back();
            stack.pop_back();
            Value obj = stack.back();
            stack.pop_back();

            std::get<GameObject *>(obj)->setPosition(sf::Vector2f(std::get<sf::Vector2f>(pos)));
        }
        break;
        case Instructions::GetPosition:
        {
            Value obj = stack.back();
            stack.pop_back();

            stack.push_back(std::get<GameObject *>(obj)->getPosition());
        }
        break;
        case Instructions::MakePosition:
        {
            Value a = stack.back();
            stack.pop_back();
            Value b = stack.back();
            stack.pop_back();
            stack.push_back(sf::Vector2f(std::get<double>(a), std::get<double>(b)));
        }
        break;
        case Instructions::GetPositionX:
            break;
        case Instructions::GetPositionY:
            break;
        }
        pos++;
    }
}