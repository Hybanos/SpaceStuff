#ifndef SPACESTUFF_OBJECT_CUBEMAP_HPP
#define SPACESTUFF_OBJECT_CUBEMAP_HPP

#include "object_faces.hpp"
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class ObjectCubeMap : virtual public ObjectFaces {
    protected:
        std::vector<std::string> files;

        GLuint texture_id;
        GLuint cubemap_id;
        GLuint matrix_id;

        GLuint rota_id;
        GLuint flip_id;

        bool draw_texture = true;
        virtual void manage_texture();
    public:
        ObjectCubeMap(std::string[6]);
        virtual void draw_t();
};

#endif