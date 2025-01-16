#ifndef SPACESTUFF_OBJECT_HPP
#define SPACESTUFF_OBJECT_HPP

#include <iostream>
#include <vector>

#include "GL/glew.h"
#include "GL/gl.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

class Object {
    public:
        virtual void draw() = 0;
        virtual void debug() = 0;
};

#endif