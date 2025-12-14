#pragma once
#include <string>
#include <map>
#include <vector>
#include "../Execution/Runnable.hpp"

namespace Engine
{
    class SceneDescriptionObject
    {
    public:
    private:
        std::string m_name;
        std::string m_typeName;
        sf::Vector2f m_startPosition;
        sf::Vector2f m_startSize;
        std::map<std::string, Runnable::CodeConstantValue> m_defaultValues;
    };

    class SceneDescription
    {
    public:
        explicit SceneDescription();

    private:
        std::vector<SceneDescriptionObject> m_objects;
    };
} // namespace Engine
