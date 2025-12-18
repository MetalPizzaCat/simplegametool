#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "../Execution/Value.hpp"

namespace Engine
{
    enum class SceneDescriptionPropValueType
    {
        Bool,
        Int,
        Float,
        String,
        Vector
    };

    using SceneDescriptionPropertyValue = std::variant<bool, IntType, FloatType, std::string, sf::Vector2f>;

    class SceneDescriptionObject
    {
    public:
        explicit SceneDescriptionObject(std::string const &name,
                                        std::string const &type,
                                        sf::Vector2f startPos,
                                        std::optional<sf::Vector2f> startSize,
                                        std::map<std::string, SceneDescriptionPropertyValue> const &props);

        std::string const &getName() const { return m_name; }

        std::string const &getTypeName() const { return m_typeName; }

        sf::Vector2f getStartPosition() const { return m_startPosition; }

        std::map<std::string, SceneDescriptionPropertyValue> const &getDefaultValues() const { return m_defaultValues; }

        virtual ~SceneDescriptionObject() = default;

    private:
        std::string m_name;
        std::string m_typeName;
        sf::Vector2f m_startPosition;
        std::optional<sf::Vector2f> m_startSize;
        std::map<std::string, SceneDescriptionPropertyValue> m_defaultValues;
    };

    class SceneDescriptionAudioObject : public SceneDescriptionObject
    {
    public:
        explicit SceneDescriptionAudioObject(std::string const &name,
                                             std::string const &audioName,
                                             sf::Vector2f startPos,
                                             std::optional<sf::Vector2f> startSize,
                                             std::map<std::string, SceneDescriptionPropertyValue> const &props);

        std::string const &getAudioName() const { return m_audio; }

    private:
        std::string m_audio;
    };

    class SceneDescriptionLabelObject : public SceneDescriptionObject
    {
    public:
        explicit SceneDescriptionLabelObject(std::string const &name,
                                             std::string const &text,
                                             std::string const &fontName,
                                             sf::Vector2f startPos,
                                             std::optional<sf::Vector2f> startSize,
                                             std::map<std::string, SceneDescriptionPropertyValue> const &props);

        std::string const &getText() const { return m_text; }
        std::string const &getFontName() const { return m_font; }

    private:
        std::string m_text;
        std::string m_font;
    };

    class SceneDescription
    {
    public:
        explicit SceneDescription(std::string const &codePath, std::optional<std::string> title, std::vector<std::unique_ptr<SceneDescriptionObject>> objects);

        std::string const &getCodePath() const { return m_codePath; }

        std::optional<std::string> getTitle() const { return m_title; }

        std::vector<std::unique_ptr<SceneDescriptionObject>> const &getObjects() const;

    private:
        std::vector<std::unique_ptr<SceneDescriptionObject>> m_objects;

        std::optional<std::string> m_title;
        std::string m_codePath;
    };
} // namespace Engine
