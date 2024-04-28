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



add_executable(pong main.cpp
        player.h
        player.cpp
        enemy.h
        enemy.cpp
        Ball.cpp
        Ball.h)
target_link_libraries(pong SDL2::SDL2)

