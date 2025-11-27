#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Asset.hpp"
namespace Engine
{
    class ContentManager
    {
    public:
        ContentManager(ContentManager const &c) = delete;
        void operator=(ContentManager const &c) = delete;
        explicit ContentManager() = default;
        static ContentManager &getInstance()
        {
            static ContentManager manager;
            return manager;
        }

        /// @brief Manually register an asset in the manager system
        /// @param name Name of the asset
        /// @param asset Asset data
        void addAsset(std::string const &name, std::unique_ptr<Asset> asset);

        Asset const *getAsset(std::string const& name) const;

        /// @brief Load texture from disk. If texture has already been loaded, pointer to existing texture is returned
        /// @param path
        /// @return
        sf::Texture *loadTexture(std::string const &path);

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::map<std::string, std::unique_ptr<Asset>> m_assets;
    };
}