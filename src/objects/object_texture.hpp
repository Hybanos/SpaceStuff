#ifndef SPACESTUFF_OBJECT_TEXTURE_HPP
#define SPACESTUFF_OBJECT_TEXTURE_HPP

#include "object.hpp"

class ObjectTexture : virtual public Object {
    protected:
        GLuint texture_id;

        bool draw_texture = true;
    public:
        virtual void draw_t() = 0;
};

#endif