cmake_minimum_required(VERSION 3.25)
project(SpaceStuff LANGUAGES CXX)

include(FetchContent)
include(ExternalProject)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm
    GIT_TAG master 
)
FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

FetchContent_Declare(
    imgui 
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.9-docking 
    GIT_SHALLOW 1
)
FetchContent_GetProperties(imgui)

if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(imgui_INCLUDE_DIR ${imgui_SOURCE_DIR})
endif()

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.10
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    glew 
    GIT_REPOSITORY https://github.com/nigels-com/glew
    GIT_TAG.glew-2.2.0
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(glew)

add_subdirectory(src)
