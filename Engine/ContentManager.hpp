#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include "Asset.hpp"
#include "AnimatedSprite.hpp"
#include "Sound.hpp"
namespace Engine
{
    class ContentManager
    {
    public:
        ContentManager(ContentManager const &c) = delete;
        void operator=(ContentManager const &c) = delete;
        explicit ContentManager() = default;
        static ContentManager &getInstance()
        {
            static ContentManager manager;
            return manager;
        }

        /// @brief Manually register a sprite animation asset in the system
        /// @param name Name of the asset
        /// @param asset Asset data
        void addSpriteAsset(std::string const &name, std::unique_ptr<SpriteFramesAsset> asset);

        /// @brief Manually register a sound asset in the system
        /// @param name Name of the asset
        /// @param asset Asset data
        void addSoundAsset(std::string const &name, std::unique_ptr<SoundAsset> asset);

        SpriteFramesAsset const *getAnimationAsset(std::string const &name) const;

        SoundAsset const *getSoundAsset(std::string const &name) const;

        /// @brief Load texture from disk. If texture has already been loaded, pointer to existing texture is returned
        /// @param path
        /// @return
        sf::Texture *loadTexture(std::string const &path);

        sf::SoundBuffer *loadSoundBuffer(std::string const &path);

        std::unique_ptr<AnimatedSprite> loadSprite(SpriteFramesAsset const *asset);

        std::unique_ptr<Sound> loadSound(SoundAsset const *asset);

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_sounds;
        std::map<std::string, std::unique_ptr<SpriteFramesAsset>> m_spriteFrameAssets;
        std::map<std::string, std::unique_ptr<SoundAsset>> m_soundAssets;
    };
}