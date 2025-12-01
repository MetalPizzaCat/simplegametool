#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include <map>
#include <iostream>
#include <variant>

#include "Engine/Scene.hpp"
#include "Code/Code.hpp"
#include "Code/Error.hpp"
#include "Engine/Error.hpp"
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

std::vector<std::string> getLines(std::string const &str)
{
    std::stringstream ss(str);
    std::vector<std::string> lines;
    std::string to;
    while (std::getline(ss, to, '\n'))
    {
        lines.push_back(to + '\n');
    }
    return lines;
}

void displayError(size_t column, size_t row, std::string const &code, std::string const &message)

{
    std::vector<std::string> lines = getLines(code);

    std::cerr << "\033[31mError at line " << row + 1 << " column " << column + 1 << ": " << message << "\033[0m" << std::endl;
    if (row > 0)
    {
        std::cerr << row << ": " << lines[row - 1];
    }
    std::string rowStr = std::to_string(row + 1);
    std::cerr << row + 1 << ": " << lines[row];
    std::cerr << std::string(((column > 3) ? column - 3 : column) + rowStr.size() + 2, ' ') << "\033[31m~~~\033[0m" << std::endl;
    if (row + 1 < lines.size())
    {
        std::cerr << row + 2 << ": " << lines[row + 1];
    }
    std::cerr << std::endl;
}

int main(int, char **)
{

    using namespace Code;

    std::string code2 = R"CODE(





type Thing  {
    sprite = @img1 # {"./assets/image.png", x = 0, y = 0, width = 16, height = 16}
    some_var = 0
    other_val = (0, 0)
    even_more = "wowza"

    func init {
        push 0
        set 1
    %l1:
        push "hello from thing!"
        print 
        get 0
        push 10
        less
        jump_if %l1
    }

    func update {
    }

    func clicked {
        
    }
}

func start {
    create_instance Thing
    set 0
}


func update{
    get 0

    get 0
    get_pos

    push 0.0
    push 1.0
    make_vector
    add

    set_pos
}
    )CODE";

    std::string code = R"CODE(
func init{ 
    push 0
    set 0
    push 4
    push 1
    less
    jump_if %end
    %l1:
        get 0
        print
        get 0
        push 1
        add
        set 0
        get 0
        push 10
        less
        jump_if %l1
    push "over"
    print
    %end:
        push "truly over"
        print
   
}

func update{
    %l1:
}
    )CODE";

    using namespace Engine;
    ContentManager::getInstance().addAsset("img1", std::make_unique<SpriteFramesAsset>("./assets/objects.png",
                                                                                       std::map<std::string, SpriteAnimation>{
                                                                                           {"default", SpriteAnimation{
                                                                                                           .frames = {
                                                                                                               sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(24, 24)),
                                                                                                               sf::IntRect(sf::Vector2i(24, 0), sf::Vector2i(24, 24)),
                                                                                                               sf::IntRect(sf::Vector2i(48, 0), sf::Vector2i(24, 24)),
                                                                                                               sf::IntRect(sf::Vector2i(72, 0), sf::Vector2i(24, 24))},
                                                                                                           .framesPerSecond = 15,
                                                                                                           .looping = true}}},
                                                                                       sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(24, 24))));
    try
    {
        Scene scene = loadSceneFromString(code);

        auto window = sf::RenderWindow(sf::VideoMode({(34 * 24u), (25 * 24u)}), "Simple game tool");
        sf::Clock deltaClock;
        window.setFramerateLimit(144);
        scene.runFunctionByName("init");
        while (window.isOpen())
        {
            sf::Time deltaTime = deltaClock.restart();
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
            }
            scene.update(deltaTime.asSeconds());

            window.clear();

            scene.draw(window);
            window.display();
        }
    }
    catch (Code::Errors::ParsingError e)
    {
        displayError(e.getColumn(), e.getLine(), code, e.what());
    }
    catch (Engine::Errors::InvalidInstructionError e)
    {
        displayError(0, 0, code, e.what());
    }
}
