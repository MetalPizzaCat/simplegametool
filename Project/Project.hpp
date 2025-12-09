#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <SFML/System.hpp>
#include "../Engine/Asset.hpp"

namespace Project
{
    /// @brief Project class represents the game project itself, from name to asset info
    class Project
    {
    public:
        /// @brief Create instance of project class by loading project information from the provided file
        /// @param path Path to the file containing project information
        explicit Project(std::string const &path);

        /// @brief Load all files in `assetFolderPath` into the global instance of content manager. Does not load underlying textures and audio files, only asset descriptions
        void loadAssetInfoIntoContentManager();

        std::string const &getName() const { return m_name; }
        std::string const &getMainScenePath() const { return m_mainScenePath; }

        void loadAsset(std::string const &path) const;

        std::string loadSceneCode(std::string const &path) const;

        sf::Vector2u getWindowSize() const { return m_windowSize; }

    protected:
        std::unique_ptr<Engine::SpriteFramesAsset> loadSpriteFramesAsset(nlohmann::json &json) const;

        std::unique_ptr<Engine::SoundAsset> loadSoundAsset(nlohmann::json const &json) const;

    private:
        std::string m_name;
        std::string m_mainScenePath;
        std::string m_assetFolderPath;
        std::string m_rootFolder;
        std::optional<std::string> m_iconPath;
        sf::Vector2u m_windowSize;
    };
} // namespace Project
