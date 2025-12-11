#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace Engine
{

    class Asset
    {

    public:
        explicit Asset(std::string const &path);
        std::string const &getPath() const { return m_path; }

        virtual ~Asset() {}

    private:
        std::string m_path;
    };

    class SoundAsset : public Asset
    {
    public:
        explicit SoundAsset(std::string const &path, float pitch, float defaultVolume, bool looping, bool positional);

        float getPitch() const { return m_pitch; }
        float getDefaultVolume() const { return m_defaultVolume; }

        bool isLooping() const { return m_looping; }
        bool isPositional() const { return m_positional; }

    private:
        float m_pitch;
        float m_defaultVolume;
        bool m_looping;
        bool m_positional;
    };

    struct SpriteAnimation
    {
        std::vector<sf::IntRect> frames;
        int32_t framesPerSecond;
        bool looping;
    };
    class SpriteFramesAsset : public Asset
    {
    public:
        explicit SpriteFramesAsset(std::string const &path, std::map<std::string, SpriteAnimation> const &frames, sf::IntRect defaultFrame);
        SpriteAnimation const &getFrames(std::string const &animName) const { return m_frames.at(animName); }

        sf::IntRect getDefaultFrame() const { return m_defaultFrame; }

        bool hasAnimation(std::string const &name) const { return m_frames.contains(name); }

    private:
        std::map<std::string, SpriteAnimation> m_frames;

        sf::IntRect m_defaultFrame;
    };
}