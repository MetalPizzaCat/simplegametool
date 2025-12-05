#include "Asset.hpp"

Engine::SpriteFramesAsset::SpriteFramesAsset(std::string const &path, std::map<std::string, SpriteAnimation> const &frames, sf::IntRect defaultFrame)
    : Asset(path), m_frames(frames), m_defaultFrame(defaultFrame)
{
}

Engine::Asset::Asset(std::string const &path) : m_path(path)
{
}
