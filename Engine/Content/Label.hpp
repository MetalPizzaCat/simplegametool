#pragma once
#include <SFML/Graphics.hpp>
namespace Engine
{
    class Label
    {
    public:
        explicit Label(sf::Text text);

    private:
        sf::Text m_text;
    };
} // namespace Engine
