cmake_minimum_required(VERSION 3.10.0)
project(simplegametool VERSION 0.1.0 LANGUAGES C CXX)

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
    Code/Error.hpp
    Code/Error.cpp
    Code/DebugInfo.hpp
    Code/DebugInfo.cpp
    Engine/Runnable.hpp
    Engine/Runnable.cpp
    Engine/Asset.hpp
    Engine/Asset.cpp
    Engine/ContentManager.hpp
    Engine/ContentManager.cpp
    Engine/GameObject.hpp
    Engine/GameObject.cpp
    Engine/ObjectType.hpp
    Engine/ObjectType.cpp
    Engine/Instructions.hpp
    Engine/Scene.hpp
    Engine/Scene.cpp
    Engine/Value.hpp
    Engine/Value.cpp
    Engine/Error.hpp
    Engine/Error.cpp
    Engine/AnimatedSprite.hpp
    Engine/AnimatedSprite.cpp
    Engine/StandardLibrary.hpp
    Engine/StandardLibrary.cpp
)

target_link_libraries(simplegametool PRIVATE SFML::Graphics SFML::Audio)


target_compile_definitions(simplegametool PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
