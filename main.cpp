#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include <map>
#include <iostream>
#include <variant>

#include "Engine/Scene.hpp"
#include "Code/Code.hpp"

class MemoryObject
{
public:
private:
};

/// @brief Wrapper around sprite class that makes it easier to use with the engine
class Sprite
{
public:
private:
    sf::Sprite m_sprite;
};

Engine::Scene loadSceneFromString(std::string const &code)
{
    Engine::Runnable::RunnableCode sceneCode = Code::Fusion::compileFusionString(code);
    return Engine::Scene(sceneCode);
}

int main(int, char **)
{

    using namespace Code;

    std::string code = R"CODE(
type Thing {
    sprite = @img1 # {"./assets/image.png", x = 0, y = 0, width = 16, height = 16}
    some_var = 0
    other_val = (0, 0)
    even_more = "wowza"
}

    )CODE";

    using namespace Engine;
    ContentManager::getInstance().addAsset("img1", std::make_unique<Asset>("./assets/objects.png", true, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(24, 24))));
    Scene scene = loadSceneFromString(code);

    // scene.addType("TestObj", std::make_unique<ObjectType>(baba.get(), std::map<std::string, ValueType>{}));
    scene.addFunction("start", 0, 0);

    scene.addFunction("update", 5, 0);

    // $0 = create_instance 0
    scene.getBytes().push_back((uint8_t)Instructions::CreateInstance);
    scene.getBytes().push_back(0);
    scene.getBytes().push_back((uint8_t)Instructions::SetLocal);
    scene.getBytes().push_back(0);
    scene.getBytes().push_back((uint8_t)Instructions::Return);

    // update:
    scene.getBytes().push_back((uint8_t)Instructions::GetLocal);
    scene.getBytes().push_back(0);

    scene.getBytes().push_back((uint8_t)Instructions::GetLocal);
    scene.getBytes().push_back(0);
    scene.getBytes().push_back((uint8_t)Instructions::GetPosition);

    scene.getBytes().push_back((uint8_t)Instructions::PushFloat);
    scene.getBytes().push_back(1);
    scene.getBytes().push_back((uint8_t)Instructions::PushFloat);
    scene.getBytes().push_back(0);
    scene.getBytes().push_back((uint8_t)Instructions::MakePosition);

    scene.getBytes().push_back((uint8_t)Instructions::Add);

    scene.getBytes().push_back((uint8_t)Instructions::SetPosition);
    scene.getBytes().push_back((uint8_t)Instructions::Return);

    auto window = sf::RenderWindow(sf::VideoMode({(34 * 24u), (25 * 24u)}), "Simple game tool");
    window.setFramerateLimit(144);
    scene.runFunction("start");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        scene.update();

        window.clear();

        scene.draw(window);
        window.display();
    }
}
