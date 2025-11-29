#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace Engine
{

    struct SpriteAnimation
    {
        std::vector<sf::IntRect> frames;
        int32_t framesPerSecond;
        bool looping;
    };
    class SpriteFramesAsset
    {
    public:
        explicit SpriteFramesAsset(std::string const &path, std::map<std::string, SpriteAnimation> const &frames, sf::IntRect defaultFrame);
        SpriteAnimation const &getFrames(std::string const &animName) const { return m_frames.at(animName); }

        sf::IntRect getDefaultFrame() const { return m_defaultFrame; }

        std::string const &getPath() const { return m_path; }

        bool hasAnimation(std::string const &name) const { return m_frames.contains(name); }

    private:
        std::map<std::string, SpriteAnimation> m_frames;
        std::string m_path;
        sf::IntRect m_defaultFrame;
    };
}