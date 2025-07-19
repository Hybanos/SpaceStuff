cmake_minimum_required(VERSION 3.25)
project(SpaceStuff LANGUAGES CXX)

include(FetchContent)
include(ExternalProject)

# TODO: install those properly if not on system
# TODO: needs sqlite3 headers and lib (libsqlite3-dev on debian) 
find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.1 
    GIT_SHALLOW 1
)
FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

FetchContent_Declare(
    imgui 
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.92.1-docking 
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

FetchContent_Declare(
    fmt 
    GIT_REPOSITORY 
    https://github.com/fmtlib/fmt 
    GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)

set(CPR_USE_SYSTEM_CURL ON)
FetchContent_Declare(
    cpr 
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.12.0 
)
FetchContent_MakeAvailable(cpr)

add_subdirectory(src)
