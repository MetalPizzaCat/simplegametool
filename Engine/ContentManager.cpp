#include "ContentManager.hpp"

void Engine::ContentManager::addAsset(std::string const &name, std::unique_ptr<Asset> asset)
{
    m_assets[name] = std::move(asset);
}

Engine::Asset const *Engine::ContentManager::getAsset(std::string const &name) const
{
    if (m_assets.contains(name))
    {
        return m_assets.at(name).get();
    }
    return nullptr;
}

sf::Texture *Engine::ContentManager::loadTexture(std::string const &path)
{
    if (m_textures.contains(path))
    {
        return m_textures.at(path).get();
    }
    m_textures[path] = std::make_unique<sf::Texture>(path);
    return m_textures[path].get();
}