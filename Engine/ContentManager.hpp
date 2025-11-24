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

        sf::Texture *loadTexture(std::string const &path)
        {
            if (m_textures.contains(path))
            {
                return m_textures.at(path).get();
            }
            m_textures[path] = std::make_unique<sf::Texture>(path);
            return m_textures[path].get();
        }

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::map<std::string, std::unique_ptr<Asset>> m_assets;
    };
}