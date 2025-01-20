#ifndef SPACESTUFF_SKYBOX_HPP
#define SPACESTUFF_SKYBOX_HPP

#include "object_cubemap.hpp"

class SkyBox : public ObjectCubeMap {
    private:
        GLuint cubemap_id;
    public:
        SkyBox(Scene *);
        void draw();
        void debug();
};

#endif