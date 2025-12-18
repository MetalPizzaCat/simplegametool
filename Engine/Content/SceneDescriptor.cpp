#include "SceneDescriptor.hpp"

Engine::SceneDescriptionObject::SceneDescriptionObject(std::string const &name,
                                                       std::string const &type,
                                                       sf::Vector2f startPos,
                                                       std::optional<sf::Vector2f> startSize,
                                                       std::map<std::string, SceneDescriptionPropertyValue> const &props) : m_name(name),
                                                                                                                            m_typeName(type),
                                                                                                                            m_startPosition(startPos),
                                                                                                                            m_startSize(startSize),
                                                                                                                            m_defaultValues(props)
{
}

Engine::SceneDescriptionAudioObject::SceneDescriptionAudioObject(std::string const &name,
                                                                 std::string const &audioName,
                                                                 sf::Vector2f startPos,
                                                                 std::optional<sf::Vector2f> startSize,
                                                                 std::map<std::string, SceneDescriptionPropertyValue> const &props)
    : SceneDescriptionObject(name, "AudioPlayer", startPos, startSize, props), m_audio(audioName)
{
}

Engine::SceneDescription::SceneDescription(std::string const &codePath,
                                           std::optional<std::string> title,
                                           std::vector<std::unique_ptr<SceneDescriptionObject>> objects)
    : m_codePath(codePath), m_title(title), m_objects(std::move(objects))
{
}

std::vector<std::unique_ptr<Engine::SceneDescriptionObject>> const &Engine::SceneDescription::getObjects() const
{
    return m_objects;
}

Engine::SceneDescriptionLabelObject::SceneDescriptionLabelObject(std::string const &name,
                                                                 std::string const &text,
                                                                 std::string const &fontName,
                                                                 sf::Vector2f startPos,
                                                                 std::optional<sf::Vector2f> startSize,
                                                                 std::map<std::string, SceneDescriptionPropertyValue> const &props)
    : SceneDescriptionObject(name, "Label", startPos, startSize, props), m_text(text), m_font(fontName)
{
}
