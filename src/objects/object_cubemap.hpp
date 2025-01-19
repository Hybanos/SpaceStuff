#ifndef SPACESTUFF_OBJECT_CUBEMAP_HPP
#define SPACESTUFF_OBJECT_CUBEMAP_HPP

#include "object.hpp"
#include "shaders/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "src/shaders/stb_image.h"

class ObjectCubeMap : virtual public Object {
    protected:
        GLuint vbo, vao;
        GLuint program_id;
        GLuint texture_id;
        GLuint matrix_id;

        bool draw_texture = true;
        virtual void manage_texture() = 0;
    public:
        ObjectCubeMap() {
            glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao);
        }
        virtual void draw_t(glm::mat4) = 0;
};

#endif