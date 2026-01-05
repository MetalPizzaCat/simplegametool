#include "ContentManager.hpp"
#include <iostream>
#include "../Error.hpp"
void Engine::ContentManager::addSpriteAsset(std::string const &name, std::unique_ptr<SpriteFramesAsset> asset)
{
    if (name == "null")
    {
        throw Errors::ContentError("Creating sprite asset with name 'null' is not allowed");
    }
    m_spriteFrameAssets[name] = std::move(asset);
}

void Engine::ContentManager::addSoundAsset(std::string const &name, std::unique_ptr<SoundAsset> asset)
{
    m_soundAssets[name] = std::move(asset);
}

void Engine::ContentManager::addFontAsset(std::string const &name, std::unique_ptr<FontAsset> asset)
{
    m_fontAssets[name] = std::move(asset);
}

Engine::SpriteFramesAsset const *Engine::ContentManager::getAnimationAsset(std::string const &name) const
{
    if (name != "null" && m_spriteFrameAssets.contains(name))
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

Engine::FontAsset const *Engine::ContentManager::getFontAsset(std::string const &name) const
{
    if (m_fontAssets.contains(name))
    {
        return m_fontAssets.at(name).get();
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
        throw Errors::ContentError(e.what());
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
        throw Errors::ContentError(e.what());
    }
}

sf::Font *Engine::ContentManager::loadFont(std::string const &path)
{
    if (m_fonts.contains(path))
    {
        return m_fonts.at(path).get();
    }
    try
    {
        m_fonts[path] = std::make_unique<sf::Font>(path);
        return m_fonts[path].get();
    }
    catch (sf::Exception e)
    {
        throw Errors::ContentError(e.what());
    }
}

sf::Font *Engine::ContentManager::getFontByAssetName(std::string const &name)
{
    FontAsset const *asset = getFontAsset(name);
    if (asset == nullptr)
    {
        throw Errors::ContentError("No font asset with name '" + name + "' found");
    }

    return loadFont(asset->getPath());
}

std::unique_ptr<Engine::AnimatedSprite> Engine::ContentManager::createSpriteFromAsset(SpriteFramesAsset const *asset)
{
    if (asset == nullptr)
    {
        return nullptr;
    }

    return std::make_unique<AnimatedSprite>(sf::Sprite(*loadTexture(asset->getPath()), asset->getDefaultFrame()), asset);
}

std::unique_ptr<Engine::Sound> Engine::ContentManager::createSoundFromAsset(SoundAsset const *asset)
{
    if (asset == nullptr)
    {
        return nullptr;
    }
    return std::make_unique<Sound>(sf::Sound(*loadSoundBuffer(asset->getPath())), asset);
}
