#include "ContentManager.hpp"
#include <iostream>
void Engine::ContentManager::addAsset(std::string const &name, std::unique_ptr<SpriteFramesAsset> asset)
{
    m_assets[name] = std::move(asset);
}

Engine::SpriteFramesAsset const *Engine::ContentManager::getAsset(std::string const &name) const
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
    try
    {
        m_textures[path] = std::make_unique<sf::Texture>(path);
        return m_textures[path].get();
    }
    catch (sf::Exception e)
    {
        std::cerr << e.what() << std::endl;
        // TODO: Replace with better error handling system or even source engine style "missing texture"
        exit(EXIT_FAILURE);
    }
}

std::unique_ptr<Engine::AnimatedSprite> Engine::ContentManager::loadSprite(SpriteFramesAsset const *asset)
{
    if (asset == nullptr)
    {
        return nullptr;
    }

    return std::make_unique<AnimatedSprite>(sf::Sprite(*loadTexture(asset->getPath()), asset->getDefaultFrame()), asset);
}
