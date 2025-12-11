#include "Asset.hpp"

Engine::SpriteFramesAsset::SpriteFramesAsset(std::string const &path, std::map<std::string, SpriteAnimation> const &frames, sf::IntRect defaultFrame)
    : Asset(path), m_frames(frames), m_defaultFrame(defaultFrame)
{
}

Engine::Asset::Asset(std::string const &path) : m_path(path)
{
}

Engine::SoundAsset::SoundAsset(std::string const &path, float pitch, float defaultVolume, bool looping, bool positional)
    : Asset(path), m_pitch(pitch), m_defaultVolume(defaultVolume), m_looping(looping), m_positional(positional)
{
}
