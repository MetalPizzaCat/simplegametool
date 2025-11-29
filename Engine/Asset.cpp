#include "Asset.hpp"

Engine::SpriteFramesAsset::SpriteFramesAsset(std::string const &path, std::map<std::string, SpriteAnimation> const &frames, sf::IntRect defaultFrame)
    : m_path(path), m_frames(frames), m_defaultFrame(defaultFrame)
{
}