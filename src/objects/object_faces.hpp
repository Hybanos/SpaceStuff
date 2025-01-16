#ifndef SPACESTUFF_OBJECT_FACES_HPP
#define SPACESTUFF_OBJECT_FACES_HPP

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

class ObjectFaces : virtual public Object {
    protected:
        GLuint triangles_buffer;
        GLuint triangles_color_buffer;
        std::vector<glm::vec3> triangles;
        std::vector<glm::vec3> triangles_colors;

        bool draw_faces = true;
    public:
        virtual void draw_f() = 0;
};

#endif