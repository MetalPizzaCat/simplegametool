#pragma once
#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
namespace Engine
{
    class TextObject : public GameObject
    {
    public:
        explicit TextObject(ObjectType const *type, std::string const &fontName, std::string const &name, Scene &scene);

        void draw(sf::RenderWindow &window) override;

        void setText(std::string const& text);

        std::string getText() const;

        void setPosition(sf::Vector2f pos) override;

        void setFontSize(IntType size);
    private:
        sf::Text m_text;
    };
} // namespace Engine
