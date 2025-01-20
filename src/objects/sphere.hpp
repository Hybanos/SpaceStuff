#ifndef SPACESTUFF_SPHERE_HPP
#define SPACESTUFF_SPHERE_HPP

#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object_cubemap.hpp"

class Sphere : public ObjectMesh, public ObjectCubeMap {
    private:
        GLuint cubemap_id;
        bool rebuild = false;
        int resolution = 25;
        float size = 1;
        float normalise_amount = 1.0f;
        void manageBuffers();
    public:
        Sphere(Scene *, std::string[6]);
        void draw();
        void build();
        void debug();
};

#endif