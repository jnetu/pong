cmake_minimum_required(VERSION 3.26)
project(pong)
include(CPM.cmake)

set(CMAKE_CXX_STANDARD 17)

# Set all outputs to be at the same location
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
link_directories(${CMAKE_BINARY_DIR})
#

CPMAddPackage(
        NAME SDL2
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-2.28.3
        VERSION 2.28.3
)
CPMAddPackage(
        NAME SDL2_mixer
        VERSION 2.0.15
        URL https://libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.zip
        DOWNLOAD_ONLY TRUE
)

if (SDL2_mixer_ADDED)
    # quick and dirty target definitions
    # might need extra configuration and install commands to work properly
    file(GLOB SDL2_mixer_SOURCES "${SDL2_mixer_SOURCE_DIR}/*.c")
    add_library(SDL2_mixer ${SDL2_mixer_SOURCES})
    target_link_libraries(SDL2_mixer SDL2::SDL2)
    target_include_directories(SDL2_mixer PUBLIC ${SDL2_mixer_SOURCE_DIR})
endif()


add_executable(pong main.cpp
        player.h
        player.cpp
        enemy.h
        enemy.cpp
        Ball.cpp
        Ball.h)
target_link_libraries(pong SDL2::SDL2)

