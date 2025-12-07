#include "Project.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "Errors.hpp"
#include "../Engine/ContentManager.hpp"

Project::Project::Project(std::string const &path)
{

    if (!std::filesystem::exists(path + "/project.json") || !std::filesystem::is_regular_file(path + "/project.json"))
    {
        throw Errors::AssetFileError("Unable to locate project info file (project.json) in folder '" + path + "'");
    }
    m_rootFolder = path;
    std::ifstream file(path + "/project.json");
    nlohmann::json json;
    file >> json;
    int version = json.at("project_file_version");
    m_name = json.at("project_name");
    m_mainScenePath = m_rootFolder + "/" + json.at("start_scene").get<std::string>();
    m_assetFolderPath = m_rootFolder + "/" + json.at("asset_folder").get<std::string>();
}

void Project::Project::loadAssetInfoIntoContentManager()
{
    if (!std::filesystem::exists(m_assetFolderPath))
    {
        throw Errors::AssetFileError("Unable to locate asset folder at '" + m_assetFolderPath + "'");
    }
    for (std::filesystem::directory_entry const &entry : std::filesystem::directory_iterator(m_assetFolderPath))
    {
        std::cout << "Loading '" << entry.path() << "'" << std::endl;
        loadAsset(entry.path().string());
    }
}

void Project::Project::loadAsset(std::string const &path) const
{
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
    {
        throw Errors::AssetFileError("Unable to locate asset at '" + path + "'");
    }
    std::ifstream assetFile(path);
    nlohmann::json fileData;
    assetFile >> fileData;
    if (!fileData.contains("type"))
    {
        throw Errors::AssetFileError("Asset file  '" + path + "' is missing asset type field");
    }
    try
    {
        if (fileData.at("type").get<std::string>() == "animation")
        {
            Engine::ContentManager::getInstance().addAsset(fileData.at("name"), loadSpriteFramesAsset(fileData));
        }
    }
    catch (nlohmann::json::exception e)
    {
        throw Errors::AssetFileError("Error in file '" + path + "': " + e.what());
    }
}

std::string Project::Project::loadSceneCode(std::string const &path) const
{
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
    {
        throw Errors::AssetFileError("Unable to locate asset at '" + path + "'");
    }
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

std::unique_ptr<Engine::SpriteFramesAsset> Project::Project::loadSpriteFramesAsset(nlohmann::json &json) const
{

    std::map<std::string, Engine::SpriteAnimation> animations;
    for (nlohmann::json::iterator it = json.at("animations").begin(); it != json.at("animations").end(); ++it)
    {
        std::string name = it->at("name");
        int fps = it->at("fps");
        bool looping = it->at("looping");
        std::vector<sf::IntRect> frames;
        for (auto const &frame : it->at("frames"))
        {
            frames.push_back(sf::IntRect(sf::Vector2i(frame.at("x"), frame.at("y")),
                                         sf::Vector2i(frame.at("w"), frame.at("h"))));
        }
        animations[name] = Engine::SpriteAnimation{.frames = frames, .framesPerSecond = fps, .looping = looping};
    }
    return std::make_unique<Engine::SpriteFramesAsset>(m_rootFolder + "/" + json.at("file_path").get<std::string>(),
                                                       animations,
                                                       sf::IntRect(
                                                           sf::Vector2i(json.at("default_rect").at("x"), json.at("default_rect").at("y")),
                                                           sf::Vector2i(json.at("default_rect").at("w"), json.at("default_rect").at("h"))));
}
