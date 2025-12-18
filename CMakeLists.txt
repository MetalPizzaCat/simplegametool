cmake_minimum_required(VERSION 3.10.0)
project(simplegametool VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(SFML)


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(simplegametool main.cpp
    Code/Code.hpp
    Code/Code.cpp
    Code/Token.hpp
    Code/Token.cpp
    Code/Parser.hpp
    Code/Parser.cpp
    Code/CodeBuilder.hpp
    Code/CodeBuilder.cpp
    Code/CodeGenerator.hpp
    Code/CodeGenerator.cpp
    Code/Lexems.hpp
    Code/Lexems.cpp
    Code/Error.hpp
    Code/Error.cpp
    Code/DebugInfo.hpp
    Code/DebugInfo.cpp


    Engine/Object/GameObject.hpp
    Engine/Object/GameObject.cpp
    Engine/Object/MemoryObject.hpp
    Engine/Object/MemoryObject.cpp
    Engine/Object/ObjectType.hpp
    Engine/Object/ObjectType.cpp
    Engine/Object/TextObject.hpp
    Engine/Object/TextObject.cpp
    Engine/Object/AudioObject.hpp
    Engine/Object/AudioObject.cpp
    Engine/Object/ArrayObject.hpp
    Engine/Object/ArrayObject.cpp

    Engine/Execution/Instructions.hpp
    Engine/Execution/Value.hpp
    Engine/Execution/Value.cpp
    Engine/Execution/Runnable.hpp
    Engine/Execution/Runnable.cpp

    Engine/Content/AnimatedSprite.hpp
    Engine/Content/AnimatedSprite.cpp
    Engine/Content/Sound.hpp
    Engine/Content/Sound.cpp
    Engine/Content/Asset.hpp
    Engine/Content/Asset.cpp
    Engine/Content/ContentManager.hpp
    Engine/Content/ContentManager.cpp
    Engine/Content/SceneDescriptor.hpp
    Engine/Content/SceneDescriptor.cpp

    Engine/System/Random.hpp
    Engine/System/Random.cpp
    Engine/System/StandardLibrary.hpp
    Engine/System/StandardLibrary.cpp

    Engine/Scene.hpp
    Engine/Scene.cpp
    Engine/Error.hpp
    Engine/Error.cpp

    Project/Project.hpp
    Project/Project.cpp
    Project/Errors.hpp
    Project/Errors.cpp
)

target_link_libraries(simplegametool PRIVATE SFML::Graphics SFML::Audio nlohmann_json::nlohmann_json)


target_compile_definitions(simplegametool PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
