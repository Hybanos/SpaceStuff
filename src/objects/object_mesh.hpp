#ifndef SPACESTUFF_OBJECT_MESH_HPP
#define SPACESTUFF_OBJECT_MESH_HPP

#include "object.hpp"

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

class ObjectMesh : virtual public Object {
    protected:
        GLuint lines_buffer;
        GLuint lines_color_buffer;
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> lines_colors;

        bool draw_mesh = true;
    public:
        virtual void draw_m() = 0;
};

#endif