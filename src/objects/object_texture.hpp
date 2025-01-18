#ifndef SPACESTUFF_OBJECT_TEXTURE_HPP
#define SPACESTUFF_OBJECT_TEXTURE_HPP

#include "object.hpp"
#include "shaders/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "src/shaders/stb_image.h"

class ObjectTexture : virtual public Object {
    protected:
        GLuint vbo, vao;
        GLuint program_id;
        GLuint texture_id;
        GLuint matrix_id;

        bool draw_texture = true;
    public:
        ObjectTexture() {
        }
        virtual void draw_t(glm::mat4) = 0;
};

#endif