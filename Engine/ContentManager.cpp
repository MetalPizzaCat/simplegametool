#include "ContentManager.hpp"
#include <iostream>

void Engine::ContentManager::addSpriteAsset(std::string const &name, std::unique_ptr<SpriteFramesAsset> asset)
{
    m_spriteFrameAssets[name] = std::move(asset);
}

void Engine::ContentManager::addSoundAsset(std::string const &name, std::unique_ptr<SoundAsset> asset)
{
    m_soundAssets[name] = std::move(asset);
}

Engine::SpriteFramesAsset const *Engine::ContentManager::getAnimationAsset(std::string const &name) const
{
    if (m_spriteFrameAssets.contains(name))
    {
        return m_spriteFrameAssets.at(name).get();
    }
    return nullptr;
}

Engine::SoundAsset const *Engine::ContentManager::getSoundAsset(std::string const &name) const
{
    if (m_soundAssets.contains(name))
    {
        return m_soundAssets.at(name).get();
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

sf::SoundBuffer *Engine::ContentManager::loadSoundBuffer(std::string const &path)
{
    if (m_sounds.contains(path))
    {
        return m_sounds.at(path).get();
    }
    try
    {
        m_sounds[path] = std::make_unique<sf::SoundBuffer>(path);
        return m_sounds[path].get();
    }
    catch (sf::Exception e)
    {
        std::cerr << e.what() << std::endl;
        // TODO: Replace with better error handling system or even source engine style "fiddle sticks, what now"
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

std::unique_ptr<Engine::Sound> Engine::ContentManager::loadSound(SoundAsset const *asset)
{
    if (asset == nullptr)
    {
        return nullptr;
    }
    return std::make_unique<Sound>(sf::Sound(*loadSoundBuffer(asset->getPath())), asset);
}
