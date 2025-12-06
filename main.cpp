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


    std::string code = R"CODE(
type Baba{
    sprite = @img1
    direction = 1.0

    func on_animation_ended (self) {
        vars a, b, d
        push "first"
        push "second"
        call_method_static Self::thing

        get $self    
        push "direction"
        get_field 
        print
        push "wow aniamtion over!"    
        print

        get $self
        call_method do_thing
    }

    func do_thing(self){
        push "indeed doing a thing!"    
        print 
    }

    func thing(a, b) {
        get $a
        print 
        get $b
        print
    }
}

func init{
    push "baba"
    create_instance Baba
    push 8.0
    call_method_static std::sqrt
    print
    get_const std::pi
    print
}

func update{
    vars baba 
    # baba = get_object("baba")
    push "baba"
    get_instance
    set $baba

    get $baba

    get $baba
    get_pos

    get $baba
    get_pos
    get_x
    push 128.0
    less 
    jump_if %back
        get $baba
        push -1.0
        push "direction"
        set_field
        jump %move
    %back:
        get $baba
        get_pos
        get_x
        push 0.0
        more
        jump_if %move
        get $baba
        push 1.0
        push "direction"
        set_field
    %move:

    push 0.0
    get $baba
    push "direction"
    get_field
    
    make_vector
    add
    
    set_pos
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
                                                                                                           .framesPerSecond = 1,
                                                                                                           .looping = false}}},
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
    catch (Engine::Errors::RuntimeError e)
    {
        displayError(e.getColumn(), e.getLine(), code, e.what());
    }
}
