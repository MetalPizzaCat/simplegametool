#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Engine
{
    class Asset
    {
    public:
        explicit Asset(std::string const &path, bool useFrame, sf::IntRect const &frame) : m_path(path), m_useFrame(useFrame), m_frame(frame) {}

        std::string const &getPath() const { return m_path; }

    private:
        std::string m_path;
        bool m_useFrame;
        sf::IntRect m_frame;
    };
}