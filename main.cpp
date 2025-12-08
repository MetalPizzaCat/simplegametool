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

#include "Project/Project.hpp"

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
    using namespace Engine;
    try
    {
        Project::Project p("./testproject");
        p.loadAssetInfoIntoContentManager();
        std::string code = p.loadSceneCode(p.getMainScenePath());

        try
        {
            Scene scene = loadSceneFromString(code);
            auto window = sf::RenderWindow(sf::VideoMode({(34 * 24u), (25 * 24u)}), p.getName());
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
                    else if (const sf::Event::KeyPressed *keyPress = event->getIf<sf::Event::KeyPressed>())
                    {
                        scene.handleKeyboardPress(keyPress->scancode);
                    }
                    else if (const sf::Event::MouseMoved *mouseMove = event->getIf<sf::Event::MouseMoved>())
                    {
                        scene.handleMouseMove(sf::Vector2f(mouseMove->position.x, mouseMove->position.y));
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
    catch (nlohmann::json::exception e)
    {
        std::cerr << "Failed to load project data " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
